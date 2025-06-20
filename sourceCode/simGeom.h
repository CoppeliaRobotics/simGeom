#pragma once

#include <geom.h>
#include <simLib-2/simExp.h>

SIM_DLLEXPORT int simInit(SSimInit*);
SIM_DLLEXPORT void simCleanup();
SIM_DLLEXPORT void simMsg(SSimMsg*);

SIM_DLLEXPORT void geomPlugin_releaseBuffer(void* buff);

// Mesh creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createMesh(const double* vertices,int verticesSize,const int* indices,int indicesSize,const double meshOrigin[7],double triangleEdgeMaxLength,int maxTrianglesInBoundingBox);
SIM_DLLEXPORT void* geomPlugin_copyMesh(const void* meshObbStruct);
SIM_DLLEXPORT void* geomPlugin_getMeshFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getMeshSerializationData(const void* meshObbStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scaleMesh(void* meshObbStruct,double scalingFactor);
SIM_DLLEXPORT void geomPlugin_destroyMesh(void* meshObbStruct);
SIM_DLLEXPORT double geomPlugin_getMeshRootObbVolume(const void* meshObbStruct);

// OC tree creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const double* points,int pointCnt,const double octreeOrigin[7],double cellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const double* points,int pointCnt,const double octreeOrigin[7],double cellS,const unsigned char* rgbData,const unsigned int* usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const void* meshObbStruct,const double meshTransformation[7],const double octreeOrigin[7],double cellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const void* otherOctreeStruct,const double otherOctreeTransformation[7],const double newOctreeOrigin[7],double newOctreeCellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_copyOctree(const void* ocStruct);
SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData(const void* ocStruct,int* serializationDataSize);
SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData_float(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData_float(const void* ocStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,double f);
SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct);
SIM_DLLEXPORT double* geomPlugin_getOctreeVoxelData(const void* ocStruct,int* voxelCount);
SIM_DLLEXPORT unsigned int* geomPlugin_getOctreeUserData(const void* ocStruct,int* voxelCount);
SIM_DLLEXPORT double* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* pointCount);

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const double octreeTransformation[7],const double* points,int pointCnt,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const double octreeTransformation[7],const double* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData);
SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const double octreeTransformation[7],const void* obbStruct,const double meshTransformation[7],const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7],const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT bool geomPlugin_removePointsFromOctree(void* ocStruct,const double octreeTransformation[7],const double* points,int pointCnt);
SIM_DLLEXPORT bool geomPlugin_removeMeshFromOctree(void* ocStruct,const double octreeTransformation[7],const void* obbStruct,const double meshTransformation[7]);
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromOctree(void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7]);

// Point cloud creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromPoints(const double* points,int pointCnt,const double ptcloudOrigin[7],double cellS,int maxPointCnt,const unsigned char rgbData[3],double proximityTol);
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromColorPoints(const double* points,int pointCnt,const double ptcloudOrigin[7],double cellS,int maxPointCnt,const unsigned char* rgbData,double proximityTol);
SIM_DLLEXPORT void* geomPlugin_copyPtcloud(const void* pcStruct);
SIM_DLLEXPORT void* geomPlugin_getPtcloudFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getPtcloudSerializationData(const void* pcStruct,int* serializationDataSize);
SIM_DLLEXPORT void* geomPlugin_getPtcloudFromSerializationData_float(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getPtcloudSerializationData_float(const void* pcStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scalePtcloud(void* pcStruct,double f);
SIM_DLLEXPORT void geomPlugin_destroyPtcloud(void* pcStruct);
SIM_DLLEXPORT double* geomPlugin_getPtcloudPoints(const void* pcStruct,int* pointCount,double prop);
SIM_DLLEXPORT double* geomPlugin_getPtcloudOctreeCorners(const void* pcStruct,int* pointCount);
SIM_DLLEXPORT int geomPlugin_getPtcloudNonEmptyCellCount(const void* pcStruct);

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,const unsigned char rgbData[3],double proximityTol);
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,const unsigned char* rgbData,double proximityTol);
SIM_DLLEXPORT bool geomPlugin_removePointsFromPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,double proximityTol,int* countRemoved);
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromPtcloud(void* pcStruct,const double ptcloudTransformation[7],const void* ocStruct,const double octreeTransformation[7],int* countRemoved);
SIM_DLLEXPORT bool geomPlugin_intersectPointsWithPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,double proximityTol);

// Collision detection
SIM_DLLEXPORT bool geomPlugin_getMeshMeshCollision(const void* mesh1ObbStruct,const double mesh1Transformation[7],const void* mesh2ObbStruct,const double mesh2Transformation[7],double** intersections,int* intersectionsSize,int* mesh1Caching,int* mesh2Caching);
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeCollision(const void* meshObbStruct,const double meshTransformation[7],const void* ocStruct,const double octreeTransformation[7],int* meshCaching,unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleCollision(const void* meshObbStruct,const double meshTransformation[7],const double p[3],const double v[3],const double w[3],double** intersections,int* intersectionsSize,int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentCollision(const void* meshObbStruct,const double meshTransformation[7],const double segmentExtremity[3],const double segmentVector[3],double** intersections,int* intersectionsSize,int* caching);

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudCollision(const void* ocStruct,const double octreeTransformation[7],const void* pcStruct,const double ptcloudTransformation[7],unsigned long long int* ocCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleCollision(const void* ocStruct,const double octreeTransformation[7],const double p[3],const double v[3],const double w[3],unsigned long long int* caching);
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentCollision(const void* ocStruct,const double octreeTransformation[7],const double segmentExtremity[3],const double segmentVector[3],unsigned long long int* caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePointsCollision(const void* ocStruct,const double octreeTransformation[7],const double* points,int pointCount);
SIM_DLLEXPORT bool geomPlugin_getOctreePointCollision(const void* ocStruct,const double octreeTransformation[7],const double point[3],unsigned int* usrData,unsigned long long int* caching);

SIM_DLLEXPORT bool geomPlugin_getBoxBoxCollision(const double box1Transformation[7],const double box1HalfSize[3],const double box2Transformation[7],const double box2HalfSize[3],bool boxesAreSolid);
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleCollision(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double p[3],const double v[3],const double w[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentCollision(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double segmentEndPoint[3],const double segmentVector[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxPointCollision(const double boxTransformation[7],const double boxHalfSize[3],const double point[3]);

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleCollision(const double p1[3],const double v1[3],const double w1[3],const double p2[3],const double v2[3],const double w2[3],double** intersections,int* intersectionsSize);
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentCollision(const double p[3],const double v[3],const double w[3],const double segmentEndPoint[3],const double segmentVector[3],double** intersections,int* intersectionsSize);

// Distance calculation
SIM_DLLEXPORT bool geomPlugin_getMeshMeshDistanceIfSmaller(const void* mesh1ObbStruct,const double mesh1Transformation[7],const void* mesh2ObbStruct,const double mesh2Transformation[7],double* dist,double minDistSegPt1[3],double minDistSegPt2[3],int* mesh1Caching,int* mesh2Caching);
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const void* ocStruct,const double octreeTransformation[7],double* dist,double meshMinDistPt[3],double ocMinDistPt[3],int* meshCaching,unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshPtcloudDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const void* pcStruct,const double pcTransformation[7],double* dist,double meshMinDistPt[3],double pcMinDistPt[3],int* meshCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const double p[3],const double v[3],const double w[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3],int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3],int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshPointDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const double point[3],double* dist,double minDistSegPt[3],int* caching);

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7],double* dist,double oc1MinDistPt[3],double oc2MinDistPt[3],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const void* pcStruct,const double pcTransformation[7],double* dist,double ocMinDistPt[3],double pcMinDistPt[3],unsigned long long int* ocCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const double p[3],const double v[3],const double w[3],double* dist,double ocMinDistPt[3],double triMinDistPt[3],unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double ocMinDistPt[3],double segMinDistPt[3],unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const double point[3],double* dist,double ocMinDistPt[3],unsigned long long int* ocCaching);

SIM_DLLEXPORT bool geomPlugin_getPtcloudPtcloudDistanceIfSmaller(const void* pc1Struct,const double pc1Transformation[7],const void* pc2Struct,const double pc2Transformation[7],double* dist,double* pc1MinDistPt,double* pc2MinDistPt,unsigned long long int* pc1Caching,unsigned long long int* pc2Caching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudTriangleDistanceIfSmaller(const void* pcStruct,const double pcTransformation[7],const double p[3],const double v[3],const double w[3],double* dist,double* pcMinDistPt,double* triMinDistPt,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudSegmentDistanceIfSmaller(const void* pcStruct,const double pcTransformation[7],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double* pcMinDistPt,double* segMinDistPt,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudPointDistanceIfSmaller(const void* pcStruct,const double pcTransformation[7],const double point[3],double* dist,double* pcMinDistPt,unsigned long long int* pcCaching);

SIM_DLLEXPORT double geomPlugin_getApproxBoxBoxDistance(const double box1Transformation[7],const double box1HalfSize[3],const double box2Transformation[7],const double box2HalfSize[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxBoxDistanceIfSmaller(const double box1Transformation[7],const double box1HalfSize[3],const double box2Transformation[7],const double box2HalfSize[3],bool boxesAreSolid,double* dist,double distSegPt1[3],double distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleDistanceIfSmaller(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double p[3],const double v[3],const double w[3],double* dist,double distSegPt1[3],double distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentDistanceIfSmaller(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double segmentEndPoint[3],const double segmentVector[3],double* dist,double distSegPt1[3],double distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxPointDistanceIfSmaller(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double point[3],double* dist,double distSegPt1[3]);

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleDistanceIfSmaller(const double p1[3],const double v1[3],const double w1[3],const double p2[3],const double v2[3],const double w2[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentDistanceIfSmaller(const double p[3],const double v[3],const double w[3],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getTrianglePointDistanceIfSmaller(const double p[3],const double v[3],const double w[3],const double point[3],double* dist,double minDistSegPt[3]);

SIM_DLLEXPORT bool geomPlugin_getSegmentSegmentDistanceIfSmaller(const double segment1EndPoint[3],const double segment1Vector[3],const double segment2EndPoint[3],const double segment2Vector[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getSegmentPointDistanceIfSmaller(const double segmentEndPoint[3],const double segmentVector[3],const double point[3],double* dist,double minDistSegPt[3]);

// Volume sensor
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectMeshIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const void* obbStruct,const double meshTransformationRelative[7],double* dist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectOctreeIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const void* ocStruct,const double octreeTransformationRelative[7],double* dist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectPtcloudIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const void* pcStruct,const double ptcloudTransformationRelative[7],double* dist,bool fast,double detectPt[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectTriangleIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const double p[3],const double v[3],const double w[3],double* dist,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectSegmentIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const double segmentEndPoint[3],const double segmentVector[3],double* dist,double maxAngle,double detectPt[3]);

// Ray sensor
SIM_DLLEXPORT bool geomPlugin_raySensorDetectMeshIfSmaller(const double rayStart[3],const double rayVect[3],const void* obbStruct,const double meshTransformationRelative[7],double* dist,double forbiddenDist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3],bool* forbiddenDistTouched);
SIM_DLLEXPORT bool geomPlugin_raySensorDetectOctreeIfSmaller(const double rayStart[3],const double rayVect[3],const void* ocStruct,const double octreeTransformationRealtive[7],double* dist,double forbiddenDist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3],bool* forbiddenDistTouched);

// Volume-pt test
SIM_DLLEXPORT bool geomPlugin_isPointInVolume(const double* planesIn,int planesInSize,const double point[3]);
