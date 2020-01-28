#ifndef SIMEXTGEOMETRIC_H
#define SIMEXTGEOMETRIC_H

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
SIM_DLLEXPORT void* geomPlugin_createMesh(const float* vertices,int verticesSize,const int* indices,int indicesSize,const float meshOrigin[7],float triangleEdgeMaxLength,int maxTrianglesInBoundingBox);
SIM_DLLEXPORT void* geomPlugin_copyMesh(const void* meshObbStruct);
SIM_DLLEXPORT void* geomPlugin_getMeshFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getMeshSerializationData(const void* meshObbStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scaleMesh(void* meshObbStruct,float scalingFactor);
SIM_DLLEXPORT void geomPlugin_destroyMesh(void* meshObbStruct);
SIM_DLLEXPORT float geomPlugin_getMeshRootObbVolume(const void* meshObbStruct);

// OC tree creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const float* points,int pointCnt,const float octreeOrigin[7],float cellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const float* points,int pointCnt,const float octreeOrigin[7],float cellS,const unsigned char* rgbData,const unsigned int* usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const void* meshObbStruct,const float meshTransformation[7],const float octreeOrigin[7],float cellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const void* otherOctreeStruct,const float otherOctreeTransformation[7],const float newOctreeOrigin[7],float newOctreeCellS,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void* geomPlugin_copyOctree(const void* ocStruct);
SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData(const void* ocStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,float f);
SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct);
SIM_DLLEXPORT float* geomPlugin_getOctreeVoxelData(const void* ocStruct,int* voxelCount);
SIM_DLLEXPORT unsigned int* geomPlugin_getOctreeUserData(const void* ocStruct,int* voxelCount);
SIM_DLLEXPORT float* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* pointCount);

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const float octreeTransformation[7],const float* points,int pointCnt,const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const float octreeTransformation[7],const float* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData);
SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const float octreeTransformation[7],const void* obbStruct,const float meshTransformation[7],const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7],const unsigned char rgbData[3],unsigned int usrData);
SIM_DLLEXPORT bool geomPlugin_removePointsFromOctree(void* ocStruct,const float octreeTransformation[7],const float* points,int pointCnt);
SIM_DLLEXPORT bool geomPlugin_removeMeshFromOctree(void* ocStruct,const float octreeTransformation[7],const void* obbStruct,const float meshTransformation[7]);
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromOctree(void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7]);

// Point cloud creation/destruction/manipulation/info
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromPoints(const float* points,int pointCnt,const float ptcloudOrigin[7],float cellS,int maxPointCnt,const unsigned char rgbData[3],float proximityTol);
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromColorPoints(const float* points,int pointCnt,const float ptcloudOrigin[7],float cellS,int maxPointCnt,const unsigned char* rgbData,float proximityTol);
SIM_DLLEXPORT void* geomPlugin_copyPtcloud(const void* pcStruct);
SIM_DLLEXPORT void* geomPlugin_getPtcloudFromSerializationData(const unsigned char* serializationData);
SIM_DLLEXPORT unsigned char* geomPlugin_getPtcloudSerializationData(const void* pcStruct,int* serializationDataSize);
SIM_DLLEXPORT void geomPlugin_scalePtcloud(void* pcStruct,float f);
SIM_DLLEXPORT void geomPlugin_destroyPtcloud(void* pcStruct);
SIM_DLLEXPORT float* geomPlugin_getPtcloudPoints(const void* pcStruct,int* pointCount,float prop);
SIM_DLLEXPORT float* geomPlugin_getPtcloudOctreeCorners(const void* pcStruct,int* pointCount);
SIM_DLLEXPORT int geomPlugin_getPtcloudNonEmptyCellCount(const void* pcStruct);

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,const unsigned char rgbData[3],float proximityTol);
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,const unsigned char* rgbData,float proximityTol);
SIM_DLLEXPORT bool geomPlugin_removePointsFromPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,float proximityTol,int* countRemoved);
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromPtcloud(void* pcStruct,const float ptcloudTransformation[7],const void* ocStruct,const float octreeTransformation[7],int* countRemoved);
SIM_DLLEXPORT bool geomPlugin_intersectPointsWithPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,float proximityTol);

// Collision detection
SIM_DLLEXPORT bool geomPlugin_getMeshMeshCollision(const void* mesh1ObbStruct,const float mesh1Transformation[7],const void* mesh2ObbStruct,const float mesh2Transformation[7],float** intersections,int* intersectionsSize,int* mesh1Caching,int* mesh2Caching);
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeCollision(const void* meshObbStruct,const float meshTransformation[7],const void* ocStruct,const float octreeTransformation[7],int* meshCaching,unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleCollision(const void* meshObbStruct,const float meshTransformation[7],const float p[3],const float v[3],const float w[3],float** intersections,int* intersectionsSize,int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentCollision(const void* meshObbStruct,const float meshTransformation[7],const float segmentExtremity[3],const float segmentVector[3],float** intersections,int* intersectionsSize,int* caching);

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudCollision(const void* ocStruct,const float octreeTransformation[7],const void* pcStruct,const float ptcloudTransformation[7],unsigned long long int* ocCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleCollision(const void* ocStruct,const float octreeTransformation[7],const float p[3],const float v[3],const float w[3],unsigned long long int* caching);
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentCollision(const void* ocStruct,const float octreeTransformation[7],const float segmentExtremity[3],const float segmentVector[3],unsigned long long int* caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePointsCollision(const void* ocStruct,const float octreeTransformation[7],const float* points,int pointCount);
SIM_DLLEXPORT bool geomPlugin_getOctreePointCollision(const void* ocStruct,const float octreeTransformation[7],const float point[3],unsigned int* usrData,unsigned long long int* caching);

SIM_DLLEXPORT bool geomPlugin_getBoxBoxCollision(const float box1Transformation[7],const float box1HalfSize[3],const float box2Transformation[7],const float box2HalfSize[3],bool boxesAreSolid);
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleCollision(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float p[3],const float v[3],const float w[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentCollision(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float segmentEndPoint[3],const float segmentVector[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxPointCollision(const float boxTransformation[7],const float boxHalfSize[3],const float point[3]);

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleCollision(const float p1[3],const float v1[3],const float w1[3],const float p2[3],const float v2[3],const float w2[3],float** intersections,int* intersectionsSize);
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentCollision(const float p[3],const float v[3],const float w[3],const float segmentEndPoint[3],const float segmentVector[3],float** intersections,int* intersectionsSize);

// Distance calculation
SIM_DLLEXPORT bool geomPlugin_getMeshMeshDistanceIfSmaller(const void* mesh1ObbStruct,const float mesh1Transformation[7],const void* mesh2ObbStruct,const float mesh2Transformation[7],float* dist,float minDistSegPt1[3],float minDistSegPt2[3],int* mesh1Caching,int* mesh2Caching);
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const void* ocStruct,const float octreeTransformation[7],float* dist,float meshMinDistPt[3],float ocMinDistPt[3],int* meshCaching,unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshPtcloudDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const void* pcStruct,const float pcTransformation[7],float* dist,float meshMinDistPt[3],float pcMinDistPt[3],int* meshCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const float p[3],const float v[3],const float w[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3],int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3],int* caching);
SIM_DLLEXPORT bool geomPlugin_getMeshPointDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const float point[3],float* dist,float minDistSegPt[3],int* caching);

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7],float* dist,float oc1MinDistPt[3],float oc2MinDistPt[3],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching);
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const void* pcStruct,const float pcTransformation[7],float* dist,float ocMinDistPt[3],float pcMinDistPt[3],unsigned long long int* ocCaching,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const float p[3],const float v[3],const float w[3],float* dist,float ocMinDistPt[3],float triMinDistPt[3],unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float ocMinDistPt[3],float segMinDistPt[3],unsigned long long int* ocCaching);
SIM_DLLEXPORT bool geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const float point[3],float* dist,float ocMinDistPt[3],unsigned long long int* ocCaching);

SIM_DLLEXPORT bool geomPlugin_getPtcloudPtcloudDistanceIfSmaller(const void* pc1Struct,const float pc1Transformation[7],const void* pc2Struct,const float pc2Transformation[7],float* dist,float* pc1MinDistPt,float* pc2MinDistPt,unsigned long long int* pc1Caching,unsigned long long int* pc2Caching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudTriangleDistanceIfSmaller(const void* pcStruct,const float pcTransformation[7],const float p[3],const float v[3],const float w[3],float* dist,float* pcMinDistPt,float* triMinDistPt,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudSegmentDistanceIfSmaller(const void* pcStruct,const float pcTransformation[7],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float* pcMinDistPt,float* segMinDistPt,unsigned long long int* pcCaching);
SIM_DLLEXPORT bool geomPlugin_getPtcloudPointDistanceIfSmaller(const void* pcStruct,const float pcTransformation[7],const float point[3],float* dist,float* pcMinDistPt,unsigned long long int* pcCaching);

SIM_DLLEXPORT float geomPlugin_getApproxBoxBoxDistance(const float box1Transformation[7],const float box1HalfSize[3],const float box2Transformation[7],const float box2HalfSize[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxBoxDistanceIfSmaller(const float box1Transformation[7],const float box1HalfSize[3],const float box2Transformation[7],const float box2HalfSize[3],bool boxesAreSolid,float* dist,float distSegPt1[3],float distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleDistanceIfSmaller(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float p[3],const float v[3],const float w[3],float* dist,float distSegPt1[3],float distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentDistanceIfSmaller(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float segmentEndPoint[3],const float segmentVector[3],float* dist,float distSegPt1[3],float distSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getBoxPointDistanceIfSmaller(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float point[3],float* dist,float distSegPt1[3]);

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleDistanceIfSmaller(const float p1[3],const float v1[3],const float w1[3],const float p2[3],const float v2[3],const float w2[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentDistanceIfSmaller(const float p[3],const float v[3],const float w[3],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getTrianglePointDistanceIfSmaller(const float p[3],const float v[3],const float w[3],const float point[3],float* dist,float minDistSegPt[3]);

SIM_DLLEXPORT bool geomPlugin_getSegmentSegmentDistanceIfSmaller(const float segment1EndPoint[3],const float segment1Vector[3],const float segment2EndPoint[3],const float segment2Vector[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3]);
SIM_DLLEXPORT bool geomPlugin_getSegmentPointDistanceIfSmaller(const float segmentEndPoint[3],const float segmentVector[3],const float point[3],float* dist,float minDistSegPt[3]);

// Volume sensor
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectMeshIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const void* obbStruct,const float meshTransformationRelative[7],float* dist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectOctreeIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const void* ocStruct,const float octreeTransformationRelative[7],float* dist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectPtcloudIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const void* pcStruct,const float ptcloudTransformationRelative[7],float* dist,bool fast,float detectPt[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectTriangleIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const float p[3],const float v[3],const float w[3],float* dist,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3]);
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectSegmentIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const float segmentEndPoint[3],const float segmentVector[3],float* dist,float maxAngle,float detectPt[3]);

// Ray sensor
SIM_DLLEXPORT bool geomPlugin_raySensorDetectMeshIfSmaller(const float rayStart[3],const float rayVect[3],const void* obbStruct,const float meshTransformationRelative[7],float* dist,float forbiddenDist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3],bool* forbiddenDistTouched);
SIM_DLLEXPORT bool geomPlugin_raySensorDetectOctreeIfSmaller(const float rayStart[3],const float rayVect[3],const void* ocStruct,const float octreeTransformationRealtive[7],float* dist,float forbiddenDist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3],bool* forbiddenDistTouched);

// Volume-pt test
SIM_DLLEXPORT bool geomPlugin_isPointInVolume(const float* planesIn,int planesInSize,const float point[3]);

#endif
