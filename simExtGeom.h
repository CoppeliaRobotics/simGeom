#ifndef SIMEXTGEOMETRIC_H
#define SIMEXTGEOMETRIC_H

#include "geom.h"

#ifdef _WIN32
    #define SIM_DLLEXPORT extern "C" __declspec(dllexport)
#endif
#if defined (__linux) || defined (__APPLE__)
    #define SIM_DLLEXPORT extern "C"
#endif

SIM_DLLEXPORT unsigned char simStart(void*,int);
SIM_DLLEXPORT void simEnd();
SIM_DLLEXPORT void* simMessage(int,int*,void*,int*);

SIM_DLLEXPORT void geomPlugin_releaseBuffer(void* buff);

// Mesh creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createMesh(const simReal* vertices,int verticesSize,const int* indices,int indicesSize,const simReal meshOrigin[7],simReal triangleEdgeMaxLength,int maxTrianglesInBoundingBox);
SIM_DLLEXPORT void* geomPlugin_copyMesh(const void* meshObbStruct);
SIM_DLLEXPORT void* geomPlugin_getMeshFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getMeshSerializationData(const void* meshObbStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scaleMesh(void* meshObbStruct,simReal scalingFactor);
SIM_DLLEXPORT void geomPlugin_destroyMesh(void* meshObbStruct);
SIM_DLLEXPORT simReal geomPlugin_getMeshRootObbVolume(const void* meshObbStruct);

// OC tree creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const simReal* points,int pointCnt,const simReal octreeOrigin[7],simReal cellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const simReal* points,int pointCnt,const simReal octreeOrigin[7],simReal cellS,const unsigned char* rgbData,const unsigned int* usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const void* meshObbStruct,const simReal meshTransformation[7],const simReal octreeOrigin[7],simReal cellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const void* otherOctreeStruct,const simReal otherOctreeTransformation[7],const simReal newOctreeOrigin[7],simReal newOctreeCellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_copyOctree(const void* ocStruct);
SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData(const void* ocStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,simReal f);
SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct);
SIM_DLLEXPORT simReal* geomPlugin_getOctreeVoxelData(const void* ocStruct,int* voxelCount);
SIM_DLLEXPORT unsigned int* geomPlugin_getOctreeUserData(const void* ocStruct,int* voxelCount);
SIM_DLLEXPORT simReal* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* pointCount);

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCnt,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData);
SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const simReal octreeTransformation[7],const void* obbStruct,const simReal meshTransformation[7],const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7],const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT bool geomPlugin_removePointsFromOctree(void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCnt);
SIM_DLLEXPORT bool geomPlugin_removeMeshFromOctree(void* ocStruct,const simReal octreeTransformation[7],const void* obbStruct,const simReal meshTransformation[7]);
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromOctree(void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7]);

// Point cloud creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromPoints(const simReal* points,int pointCnt,const simReal ptcloudOrigin[7],simReal cellS,int maxPointCnt,const unsigned char rgbData[3],simReal proximityTol);
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromColorPoints(const simReal* points,int pointCnt,const simReal ptcloudOrigin[7],simReal cellS,int maxPointCnt,const unsigned char* rgbData,simReal proximityTol);
SIM_DLLEXPORT void* geomPlugin_copyPtcloud(const void* pcStruct);
SIM_DLLEXPORT void* geomPlugin_getPtcloudFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getPtcloudSerializationData(const void* pcStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scalePtcloud(void* pcStruct,simReal f);
SIM_DLLEXPORT void geomPlugin_destroyPtcloud(void* pcStruct);
SIM_DLLEXPORT simReal* geomPlugin_getPtcloudPoints(const void* pcStruct,int* pointCount,simReal prop);
SIM_DLLEXPORT simReal* geomPlugin_getPtcloudOctreeCorners(const void* pcStruct,int* pointCount);
SIM_DLLEXPORT int geomPlugin_getPtcloudNonEmptyCellCount(const void* pcStruct);

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,const unsigned char rgbData[3],simReal proximityTol);
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,const unsigned char* rgbData,simReal proximityTol);
SIM_DLLEXPORT bool geomPlugin_removePointsFromPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,simReal proximityTol,int* countRemoved);
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const void* ocStruct,const simReal octreeTransformation[7],int* countRemoved);
SIM_DLLEXPORT bool geomPlugin_intersectPointsWithPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,simReal proximityTol);

// Collision detection
SIM_DLLEXPORT bool geomPlugin_getMeshMeshCollision(const void* mesh1ObbStruct,const simReal mesh1Transformation[7],const void* mesh2ObbStruct,const simReal mesh2Transformation[7],simReal** intersections,int* intersectionsSize,int* mesh1Caching,int* mesh2Caching);
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeCollision(const void* meshObbStruct,const simReal meshTransformation[7],const void* ocStruct,const simReal octreeTransformation[7],int* meshCaching,unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleCollision(const void* meshObbStruct,const simReal meshTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal** intersections,int* intersectionsSize,int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentCollision(const void* meshObbStruct,const simReal meshTransformation[7],const simReal segmentExtremity[3],const simReal segmentVector[3],simReal** intersections,int* intersectionsSize,int* caching);

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudCollision(const void* ocStruct,const simReal octreeTransformation[7],const void* pcStruct,const simReal ptcloudTransformation[7],unsigned long long int* ocCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],unsigned long long int* caching);
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal segmentExtremity[3],const simReal segmentVector[3],unsigned long long int* caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePointsCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCount);
SIM_DLLEXPORT bool geomPlugin_getOctreePointCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal point[3],unsigned int* usrData,unsigned long long int* caching);

SIM_DLLEXPORT bool geomPlugin_getBoxBoxCollision(const simReal box1Transformation[7],const simReal box1HalfSize[3],const simReal box2Transformation[7],const simReal box2HalfSize[3],bool boxesAreSolid);
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleCollision(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal p[3],const simReal v[3],const simReal w[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentCollision(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal segmentEndPoint[3],const simReal segmentVector[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxPointCollision(const simReal boxTransformation[7],const simReal boxHalfSize[3],const simReal point[3]);

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleCollision(const simReal p1[3],const simReal v1[3],const simReal w1[3],const simReal p2[3],const simReal v2[3],const simReal w2[3],simReal** intersections,int* intersectionsSize);
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentCollision(const simReal p[3],const simReal v[3],const simReal w[3],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal** intersections,int* intersectionsSize);

// Distance calculation
SIM_DLLEXPORT bool geomPlugin_getMeshMeshDistanceIfSmaller(const void* mesh1ObbStruct,const simReal mesh1Transformation[7],const void* mesh2ObbStruct,const simReal mesh2Transformation[7],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3],int* mesh1Caching,int* mesh2Caching);
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const void* ocStruct,const simReal octreeTransformation[7],simReal* dist,simReal meshMinDistPt[3],simReal ocMinDistPt[3],int* meshCaching,unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshPtcloudDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const void* pcStruct,const simReal pcTransformation[7],simReal* dist,simReal meshMinDistPt[3],simReal pcMinDistPt[3],int* meshCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3],int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3],int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshPointDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const simReal point[3],simReal* dist,simReal minDistSegPt[3],int* caching);

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7],simReal* dist,simReal oc1MinDistPt[3],simReal oc2MinDistPt[3],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const void* pcStruct,const simReal pcTransformation[7],simReal* dist,simReal ocMinDistPt[3],simReal pcMinDistPt[3],unsigned long long int* ocCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal ocMinDistPt[3],simReal triMinDistPt[3],unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal ocMinDistPt[3],simReal segMinDistPt[3],unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const simReal point[3],simReal* dist,simReal ocMinDistPt[3],unsigned long long int* ocCaching);

SIM_DLLEXPORT bool geomPlugin_getPtcloudPtcloudDistanceIfSmaller(const void* pc1Struct,const simReal pc1Transformation[7],const void* pc2Struct,const simReal pc2Transformation[7],simReal* dist,simReal* pc1MinDistPt,simReal* pc2MinDistPt,unsigned long long int* pc1Caching,unsigned long long int* pc2Caching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudTriangleDistanceIfSmaller(const void* pcStruct,const simReal pcTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal* pcMinDistPt,simReal* triMinDistPt,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudSegmentDistanceIfSmaller(const void* pcStruct,const simReal pcTransformation[7],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal* pcMinDistPt,simReal* segMinDistPt,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudPointDistanceIfSmaller(const void* pcStruct,const simReal pcTransformation[7],const simReal point[3],simReal* dist,simReal* pcMinDistPt,unsigned long long int* pcCaching);

SIM_DLLEXPORT simReal geomPlugin_getApproxBoxBoxDistance(const simReal box1Transformation[7],const simReal box1HalfSize[3],const simReal box2Transformation[7],const simReal box2HalfSize[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxBoxDistanceIfSmaller(const simReal box1Transformation[7],const simReal box1HalfSize[3],const simReal box2Transformation[7],const simReal box2HalfSize[3],bool boxesAreSolid,simReal* dist,simReal distSegPt1[3],simReal distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleDistanceIfSmaller(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal distSegPt1[3],simReal distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentDistanceIfSmaller(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal distSegPt1[3],simReal distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxPointDistanceIfSmaller(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal point[3],simReal* dist,simReal distSegPt1[3]);

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleDistanceIfSmaller(const simReal p1[3],const simReal v1[3],const simReal w1[3],const simReal p2[3],const simReal v2[3],const simReal w2[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentDistanceIfSmaller(const simReal p[3],const simReal v[3],const simReal w[3],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getTrianglePointDistanceIfSmaller(const simReal p[3],const simReal v[3],const simReal w[3],const simReal point[3],simReal* dist,simReal minDistSegPt[3]);

SIM_DLLEXPORT bool geomPlugin_getSegmentSegmentDistanceIfSmaller(const simReal segment1EndPoint[3],const simReal segment1Vector[3],const simReal segment2EndPoint[3],const simReal segment2Vector[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getSegmentPointDistanceIfSmaller(const simReal segmentEndPoint[3],const simReal segmentVector[3],const simReal point[3],simReal* dist,simReal minDistSegPt[3]);

// Volume sensor
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectMeshIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const void* obbStruct,const simReal meshTransformationRelative[7],simReal* dist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectOctreeIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const void* ocStruct,const simReal octreeTransformationRelative[7],simReal* dist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectPtcloudIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const void* pcStruct,const simReal ptcloudTransformationRelative[7],simReal* dist,bool fast,simReal detectPt[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectTriangleIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectSegmentIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal maxAngle,simReal detectPt[3]);

// Ray sensor
SIM_DLLEXPORT bool geomPlugin_raySensorDetectMeshIfSmaller(const simReal rayStart[3],const simReal rayVect[3],const void* obbStruct,const simReal meshTransformationRelative[7],simReal* dist,simReal forbiddenDist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3],bool* forbiddenDistTouched);
SIM_DLLEXPORT bool geomPlugin_raySensorDetectOctreeIfSmaller(const simReal rayStart[3],const simReal rayVect[3],const void* ocStruct,const simReal octreeTransformationRealtive[7],simReal* dist,simReal forbiddenDist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3],bool* forbiddenDistTouched);

// Volume-pt test
SIM_DLLEXPORT bool geomPlugin_isPointInVolume(const simReal* planesIn,int planesInSize,const simReal point[3]);

#endif
