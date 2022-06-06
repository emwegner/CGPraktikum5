//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
    
}
Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
    bool ret = load(ModelFile, FitSize);
    if(!ret)
        throw std::exception();
}
Model::~Model()
{
	// TODO: Add your code (Exercise 3)
    deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
    if(!pNode)
        return;
    for(unsigned int i=0; i<pNode->ChildCount; ++i)
        deleteNodes(&(pNode->Children[i]));
    if(pNode->ChildCount>0)
        delete [] pNode->Children;
    if(pNode->MeshCount>0)
        delete [] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize)
{
    const aiScene* pScene = aiImportFile( ModelFile,aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords );
    
    if(pScene==NULL || pScene->mNumMeshes<=0)
        return false;
    
    Filepath = ModelFile;
    Path = Filepath;
    size_t pos = Filepath.rfind('/');
    if(pos == std::string::npos)
        pos = Filepath.rfind('\\');
    if(pos !=std::string::npos)
        Path.resize(pos+1);
    
    loadMeshes(pScene, FitSize);
    loadMaterials(pScene);
    loadNodes(pScene);
    
    return true;
}
void Model::loadMeshes(const aiScene* pScene, bool FitSize)
{
    if (FitSize) {
        Matrix matrix;
        matrix.scale(5);
        transform(matrix);
    }

    MeshCount = pScene->mNumMeshes;
    pMeshes = new Mesh[MeshCount];

    for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
        Mesh& newMesh = pMeshes[i];
        aiMesh* currMesh = pScene->mMeshes[i];
        newMesh.MaterialIdx = currMesh->mMaterialIndex;

        newMesh.VB.begin();
        newMesh.IB.begin();

        for (unsigned int l = 0; l < currMesh->mNumVertices; l++) {
            aiVector3D pos = currMesh->mVertices[l];
            aiVector3D normal = currMesh->mNormals[l];
            aiVector3D textCoor = currMesh->mTextureCoords[0][l];

            Vector position = Vector(pos.x, pos.y, pos.z);
            Vector normale = Vector(normal.x, normal.y, normal.z);
            Vector UV = Vector(textCoor.x, textCoor.y, textCoor.z);

            if (currMesh->HasNormals()) pMeshes[i].VB.addNormal(normale);
            if (currMesh->HasTextureCoords(0)) pMeshes[i].VB.addTexcoord1(textCoor.x, -textCoor.y);
            if (currMesh->HasPositions()) pMeshes[i].VB.addVertex(position);
        }

        for (unsigned int j = 0; j < currMesh->mNumFaces; j++) {
            aiFace currFace = currMesh->mFaces[j];

            for (unsigned int l = 0; l < currFace.mNumIndices; l++) {
                newMesh.IB.addIndex(currFace.mIndices[l]);
            }
        }

        newMesh.IB.end();
        newMesh.VB.end();
    }

    calcBoundingBox(pScene, BoundingBox);

}

void Model::loadMaterials(const aiScene* pScene)
{
    MaterialCount = pScene->mNumMaterials;
    pMaterials = new Material[MaterialCount];

    aiColor4D diffuse;
    aiColor4D ambient;
    aiColor4D specular;
    float specExp;
    aiString texName;

    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        aiMaterial* currM = pScene->mMaterials[i];
        aiGetMaterialColor(currM, AI_MATKEY_COLOR_AMBIENT, &ambient);
        aiGetMaterialColor(currM, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
        aiGetMaterialColor(currM, AI_MATKEY_COLOR_SPECULAR, &specular);
        aiGetMaterialFloat(currM, AI_MATKEY_SHININESS, &specExp);
        //  aiGetMaterialString(currM, AI_MATKEY_NAME, &texName);
        currM->GetTexture(aiTextureType_DIFFUSE, 0, &texName);

        Color amb = Color(ambient.r, ambient.g, ambient.b);
        Color spec = Color(specular.r, specular.g, specular.b);
        Color diff = Color(diffuse.r, diffuse.g, diffuse.b);


        std::string fullPath = Path + texName.C_Str();
        Material tempMat = Material();
        tempMat.DiffTex = Texture::LoadShared(fullPath.c_str());

        tempMat.AmbColor = amb;
        tempMat.DiffColor = diff;
        tempMat.SpecColor = spec;
        tempMat.SpecExp = specExp;


        pMaterials[i] = tempMat;

    }

}
void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();

    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();

    for (unsigned int i = 0; i < MeshCount; i++) {
        aiMesh* curr = pScene->mMeshes[i];
        for (unsigned int j = 0; j < MeshCount; j++) {
            float currX = curr->mVertices->x;
            float currY = curr->mVertices->y;
            float currZ = curr->mVertices->z;

            if (currX < minX) minX = currX;
            if (currY < minY) minY = currY;
            if (currZ < minZ) minZ = currZ;

            if (currX > maxX) maxX = currX;
            if (currY > maxY) maxY = currY;
            if (currZ > maxZ) maxZ = currZ;
        }
    }
    Vector min = Vector(minX, minY, minZ);
    Vector max = Vector(maxX, maxY, maxZ);

    const AABB box = AABB(min, max);
    BoundingBox = box;


}

void Model::loadNodes(const aiScene* pScene)
{
    deleteNodes(&RootNode);
    copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
    pNode->Name = paiNode->mName.C_Str();
    pNode->Trans = convert(paiNode->mTransformation);
    
    if(paiNode->mNumMeshes > 0)
    {
        pNode->MeshCount = paiNode->mNumMeshes;
        pNode->Meshes = new int[pNode->MeshCount];
        for(unsigned int i=0; i<pNode->MeshCount; ++i)
            pNode->Meshes[i] = (int)paiNode->mMeshes[i];
    }
    
    if(paiNode->mNumChildren <=0)
        return;
    
    pNode->ChildCount = paiNode->mNumChildren;
    pNode->Children = new Node[pNode->ChildCount];
    for(unsigned int i=0; i<paiNode->mNumChildren; ++i)
    {
        copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
        pNode->Children[i].Parent = pNode;
    }
}

void Model::applyMaterial( unsigned int index)
{
    if(index>=MaterialCount)
        return;
    
    PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
    if(!pPhong) {
        std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
        return;
    }
    
    Material* pMat = &pMaterials[index];
    pPhong->ambientColor(pMat->AmbColor);
    pPhong->diffuseColor(pMat->DiffColor);
    pPhong->specularExp(pMat->SpecExp);
    pPhong->specularColor(pMat->SpecColor);
    pPhong->diffuseTexture(pMat->DiffTex);
}

void Model::draw(const BaseCamera& Cam)
{
    if(!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }
    pShader->modelTransform(transform());
    
    std::list<Node*> DrawNodes;
    DrawNodes.push_back(&RootNode);
    
    while(!DrawNodes.empty())
    {
        Node* pNode = DrawNodes.front();
        Matrix GlobalTransform;
        
        if(pNode->Parent != NULL)
            pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
        else
            pNode->GlobalTrans = transform() * pNode->Trans;
        
        pShader->modelTransform(pNode->GlobalTrans);
    
        for(unsigned int i = 0; i<pNode->MeshCount; ++i )
        {
            Mesh& mesh = pMeshes[pNode->Meshes[i]];
            mesh.VB.activate();
            mesh.IB.activate();
            applyMaterial(mesh.MaterialIdx);
            pShader->activate(Cam);
            glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
            mesh.IB.deactivate();
            mesh.VB.deactivate();
        }
        for(unsigned int i = 0; i<pNode->ChildCount; ++i )
            DrawNodes.push_back(&(pNode->Children[i]));
        
        DrawNodes.pop_front();
    }
}

Matrix Model::convert(const aiMatrix4x4& m)
{
    return Matrix(m.a1, m.a2, m.a3, m.a4,
                  m.b1, m.b2, m.b3, m.b4,
                  m.c1, m.c2, m.c3, m.c4,
                  m.d1, m.d2, m.d3, m.d4);
}



