#include "simGeom.h"
#include <simLib/simLib.h>
#include <iostream>
#include <cstdio>
#include <simLib/scriptFunctionData.h>
#include <map>

#ifdef _WIN32
#ifdef QT_COMPIL
    #include <direct.h>
#else
    #include <shlwapi.h>
    #pragma comment(lib, "Shlwapi.lib")
#endif
#endif
#if defined (__linux) || defined (__APPLE__)
    #include <unistd.h>
#endif

static LIBRARY simLib;

static std::map<int,CObbStruct*> _meshData;
static int _nextMeshDataHandle=0;
static std::map<int,COcStruct*> _octreeData;
static int _nextOctreeDataHandle=0;
static std::map<int,CPcStruct*> _ptcloudData;
static int _nextPtcloudDataHandle=0;

/*
Following not (yet) implemented in the Lua API:
simGeom.getBoxBoxCollision
simGeom.getBoxTriangleCollision
simGeom.getBoxSegmentCollision
simGeom.getBoxPointCollision
simGeom.getTriangleTriangleCollision
simGeom.getTriangleSegmentCollision
simGeom.insertColorPointsIntoOctree
simGeom.insertColorPointsIntoPtcloud
simGeom.insertMeshIntoOctree
simGeom.insertOctreeIntoOctree
simGeom.insertPointsIntoOctree
simGeom.insertPointsIntoPointcloud
simGeom.intersectPointsWithPtcloud
simGeom.isPointInVolume
simGeom.raySensorDetectMesh
simGeom.raySensorDetectOctree
simGeom.removeMeshFromOctree
simGeom.removeOctreeFromOctree
simGeom.removePointsFromOctree
simGeom.removeOctreeFromPtcloud
simGeom.removePointsFromPtcloud
simGeom.volumeSensorDetectMesh
simGeom.volumeSensorDetectOctree
simGeom.volumeSensorDetectPtcloud
simGeom.volumeSensorDetectSegment
simGeom.volumeSensorDetectTriangle
*/

// --------------------------------------------------------------------------------------
// simGeom.getMeshMeshCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHMESHCOLLISION[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32|sim_lua_arg_table,2, // cachData, default is nil
    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETMESHMESHCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHMESHCOLLISION,inArgs_GETMESHMESHCOLLISION[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,CObbStruct*>::iterator it2=_meshData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_meshData.end()) )
        {
            C3Vector mesh1Pos(&(inData->at(1).doubleData[0]));
            C4Vector mesh1Q(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector mesh2Pos(&(inData->at(4).doubleData[0]));
            C4Vector mesh2Q(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            int cache1=-1;
            int cache2=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()>=2) )
            {
                cache1=inData->at(6).int32Data[0];
                cache2=inData->at(6).int32Data[1];
            }
            bool returnIntersections=false;
            if ( (inData->size()>=8)&&(inData->at(7).boolData.size()==1) )
                returnIntersections=inData->at(7).boolData[0];
            std::vector<double>* _intersect=nullptr;
            std::vector<double> intersect;
            if (returnIntersections)
                _intersect=&intersect;
            bool coll=geom_getMeshMeshCollision(it1->second,C7Vector(mesh1Q,mesh1Pos),it2->second,C7Vector(mesh2Q,mesh2Pos),_intersect,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<int> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
                if (returnIntersections)
                    D.pushOutData(CScriptFunctionDataItem(intersect));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshOctreeCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHOCTREECOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHOCTREECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHOCTREECOLLISION,inArgs_GETMESHOCTREECOLLISION[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector octreePos(&(inData->at(4).doubleData[0]));
            C4Vector octreeQ(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            int cache1=-1;
            unsigned long long int cache2=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()>=2) )
            {
                cache1=static_cast<int>(inData->at(6).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(6).doubleData[1]);
            }
            bool coll=geom_getMeshOctreeCollision(it1->second,C7Vector(meshQ,meshPos),it2->second,C7Vector(octreeQ,octreePos),&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// simGeom.getMeshTriangleCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHTRIANGLECOLLISION[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_int32,0, // cachData, default is -1
    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETMESHTRIANGLECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHTRIANGLECOLLISION,inArgs_GETMESHTRIANGLECOLLISION[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector triPt1(&inData->at(3).doubleData[0]);
            C3Vector triPt2(&inData->at(4).doubleData[0]);
            C3Vector triPt3(&inData->at(5).doubleData[0]);
            int cache=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()==1) )
                cache=inData->at(6).int32Data[0];
            bool returnIntersections=false;
            if ( (inData->size()>=8)&&(inData->at(7).boolData.size()==1) )
                returnIntersections=inData->at(7).boolData[0];
            std::vector<double>* _intersect=nullptr;
            std::vector<double> intersect;
            if (returnIntersections)
                _intersect=&intersect;
            bool coll=geom_getMeshTriangleCollision(it->second,C7Vector(meshQ,meshPos),triPt1,triPt2-triPt1,triPt3-triPt1,_intersect,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                D.pushOutData(CScriptFunctionDataItem(cache));
                if (returnIntersections)
                    D.pushOutData(CScriptFunctionDataItem(intersect));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshSegmentCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHSEGMENTCOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_int32,0, // cachData, default is -1
    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETMESHSEGMENTCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHSEGMENTCOLLISION,inArgs_GETMESHSEGMENTCOLLISION[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector segPt1(&inData->at(3).doubleData[0]);
            C3Vector segPt2(&inData->at(4).doubleData[0]);
            int cache=-1;
            if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
                cache=inData->at(5).int32Data[0];
            bool returnIntersections=false;
            if ( (inData->size()>=7)&&(inData->at(6).boolData.size()==1) )
                returnIntersections=inData->at(6).boolData[0];
            std::vector<double>* _intersect=nullptr;
            std::vector<double> intersect;
            if (returnIntersections)
                _intersect=&intersect;
            bool coll=geom_getMeshSegmentCollision(it->second,C7Vector(meshQ,meshPos),segPt1,segPt2-segPt1,_intersect,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                D.pushOutData(CScriptFunctionDataItem(cache));
                if (returnIntersections)
                    D.pushOutData(CScriptFunctionDataItem(intersect));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeOctreeCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEOCTREECOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
//    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETOCTREEOCTREECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEOCTREECOLLISION,inArgs_GETOCTREEOCTREECOLLISION[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector octree1Pos(&(inData->at(1).doubleData[0]));
            C4Vector octree1Q(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector octree2Pos(&(inData->at(4).doubleData[0]));
            C4Vector octree2Q(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()>=2) )
            {
                cache1=static_cast<unsigned long long int>(inData->at(6).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(6).doubleData[1]);
            }
            bool coll=geom_getOctreeOctreeCollision(it1->second,C7Vector(octree1Q,octree1Pos),it2->second,C7Vector(octree2Q,octree2Pos),&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePtcloudCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEPTCLOUDCOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
//    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETOCTREEPTCLOUDCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPTCLOUDCOLLISION,inArgs_GETOCTREEPTCLOUDCOLLISION[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector ptcloudPos(&(inData->at(4).doubleData[0]));
            C4Vector ptcloudQ(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()>=2) )
            {
                cache1=static_cast<unsigned long long int>(inData->at(6).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(6).doubleData[1]);
            }
            bool coll=geom_getOctreePtcloudCollision(it1->second,C7Vector(octreeQ,octreePos),it2->second,C7Vector(ptcloudQ,ptcloudPos),&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeTriangleCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREETRIANGLECOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREETRIANGLECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREETRIANGLECOLLISION,inArgs_GETOCTREETRIANGLECOLLISION[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector triPt1(&(inData->at(3).doubleData[0]));
            C3Vector triPt2(&(inData->at(4).doubleData[0]));
            C3Vector triPt3(&(inData->at(5).doubleData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(6).doubleData[0]);
            bool coll=geom_getOctreeTriangleCollision(it->second,C7Vector(octreeQ,octreePos),triPt1,triPt2-triPt1,triPt3-triPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeSegmentCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREESEGMENTCOLLISION[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREESEGMENTCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREESEGMENTCOLLISION,inArgs_GETOCTREESEGMENTCOLLISION[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector segPt1(&(inData->at(3).doubleData[0]));
            C3Vector segPt2(&(inData->at(4).doubleData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(5).doubleData[0]);
            bool coll=geom_getOctreeSegmentCollision(it->second,C7Vector(octreeQ,octreePos),segPt1,segPt2-segPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePointCollision
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEPOINTCOLLISION[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREEPOINTCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPOINTCOLLISION,inArgs_GETOCTREEPOINTCOLLISION[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector point(&(inData->at(3).doubleData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(4).doubleData[0]);
            bool coll=geom_getOctreePointCollision(it->second,C7Vector(octreeQ,octreePos),point,nullptr,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------
// simGeom.getMeshMeshDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHMESHDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_int32|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHMESHDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHMESHDISTANCE,inArgs_GETMESHMESHDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,CObbStruct*>::iterator it2=_meshData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_meshData.end()) )
        {
            C3Vector mesh1Pos(&(inData->at(1).doubleData[0]));
            C4Vector mesh1Q(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector mesh2Pos(&(inData->at(4).doubleData[0]));
            C4Vector mesh2Q(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            int cache1=-1;
            int cache2=-1;
            if ( (inData->size()>=8)&&(inData->at(7).int32Data.size()>=2) )
            {
                cache1=inData->at(7).int32Data[0];
                cache2=inData->at(7).int32Data[1];
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshMeshDistanceIfSmaller(it1->second,C7Vector(mesh1Q,mesh1Pos),it2->second,C7Vector(mesh2Q,mesh2Pos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<int> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshOctreeDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHOCTREEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHOCTREEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHOCTREEDISTANCE,inArgs_GETMESHOCTREEDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector octreePos(&(inData->at(4).doubleData[0]));
            C4Vector octreeQ(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            int cache1=-1;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=int(inData->at(7).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(7).doubleData[1]);
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshOctreeDistanceIfSmaller(it1->second,C7Vector(meshQ,meshPos),it2->second,C7Vector(octreeQ,octreePos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshPtcloudDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHPTCLOUDDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHPTCLOUDDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHPTCLOUDDISTANCE,inArgs_GETMESHPTCLOUDDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector ptcloudPos(&(inData->at(4).doubleData[0]));
            C4Vector ptcloudQ(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            int cache1=-1;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=int(inData->at(7).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(7).doubleData[1]);
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshPtcloudDistanceIfSmaller(it1->second,C7Vector(meshQ,meshPos),it2->second,C7Vector(ptcloudQ,ptcloudPos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeOctreeDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEOCTREEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETOCTREEOCTREEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEOCTREEDISTANCE,inArgs_GETOCTREEOCTREEDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector octree1Pos(&(inData->at(1).doubleData[0]));
            C4Vector octree1Q(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector octree2Pos(&(inData->at(4).doubleData[0]));
            C4Vector octree2Q(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=static_cast<unsigned long long int>(inData->at(7).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(7).doubleData[1]);
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeOctreeDistanceIfSmaller(it1->second,C7Vector(octree1Q,octree1Pos),it2->second,C7Vector(octree2Q,octree2Pos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePtcloudDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEPTCLOUDDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETOCTREEPTCLOUDDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPTCLOUDDISTANCE,inArgs_GETOCTREEPTCLOUDDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector ptcloudPos(&(inData->at(4).doubleData[0]));
            C4Vector ptcloudQ(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=static_cast<unsigned long long int>(inData->at(7).doubleData[0]);
                cache2=static_cast<unsigned long long int>(inData->at(7).doubleData[1]);
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreePtcloudDistanceIfSmaller(it1->second,C7Vector(octreeQ,octreePos),it2->second,C7Vector(ptcloudQ,ptcloudPos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeTriangleDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREETRIANGLEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREETRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREETRIANGLEDISTANCE,inArgs_GETOCTREETRIANGLEDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector triPt1(&(inData->at(3).doubleData[0]));
            C3Vector triPt2(&(inData->at(4).doubleData[0]));
            C3Vector triPt3(&(inData->at(5).doubleData[0]));
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(7).doubleData[0]);
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeTriangleDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeSegmentDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREESEGMENTDISTANCE[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREESEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREESEGMENTDISTANCE,inArgs_GETOCTREESEGMENTDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector segPt1(&(inData->at(3).doubleData[0]));
            C3Vector segPt2(&(inData->at(4).doubleData[0]));
            double dist=DBL_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1)&&(inData->at(5).doubleData[0]!=0.0) )
                dist=inData->at(5).doubleData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(6).doubleData[0]);
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeSegmentDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePointDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEPOINTDISTANCE[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREEPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPOINTDISTANCE,inArgs_GETOCTREEPOINTDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).doubleData[0]));
            C4Vector octreeQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector point(&(inData->at(3).doubleData[0]));
            double dist=DBL_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1)&&(inData->at(4).doubleData[0]!=0.0) )
                dist=inData->at(4).doubleData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(5).doubleData[0]);
            C3Vector distPt;
            bool smaller=geom_getOctreePointDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),point,dist,&distPt,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt.data,distPt.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshTriangleDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHTRIANGLEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_int32,0, // cachData, default is -1
};

void LUA_GETMESHTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHTRIANGLEDISTANCE,inArgs_GETMESHTRIANGLEDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector triPt1(&inData->at(3).doubleData[0]);
            C3Vector triPt2(&inData->at(4).doubleData[0]);
            C3Vector triPt3(&inData->at(5).doubleData[0]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            int cache=-1;
            if ( (inData->size()>=8)&&(inData->at(7).int32Data.size()==1) )
                cache=inData->at(7).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshTriangleDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(cache));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshSegmentDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHSEGMENTDISTANCE[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_int32,0, // cachData, default is -1
};

void LUA_GETMESHSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHSEGMENTDISTANCE,inArgs_GETMESHSEGMENTDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector segPt1(&inData->at(3).doubleData[0]);
            C3Vector segPt2(&inData->at(4).doubleData[0]);
            double dist=DBL_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1)&&(inData->at(5).doubleData[0]!=0.0) )
                dist=inData->at(5).doubleData[0];
            int cache=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()==1) )
                cache=inData->at(6).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshSegmentDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(cache));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshPointDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHPOINTDISTANCE[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_int32,0, // cachData, default is -1
};

void LUA_GETMESHPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHPOINTDISTANCE,inArgs_GETMESHPOINTDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector pt1(&inData->at(3).doubleData[0]);
            double dist=DBL_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1)&&(inData->at(4).doubleData[0]!=0.0) )
                dist=inData->at(4).doubleData[0];
            int cache=-1;
            if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
                cache=inData->at(5).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshPointDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),pt1,dist,&distPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem(cache));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxBoxDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETBOXBOXDISTANCE[]={
    8,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_bool,0,
};

void LUA_GETBOXBOXDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXBOXDISTANCE,inArgs_GETBOXBOXDISTANCE[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector box1Pos(&(inData->at(0).doubleData[0]));
        C4Vector box1Q(inData->at(1).doubleData[3],inData->at(1).doubleData[0],inData->at(1).doubleData[1],inData->at(1).doubleData[2]);
        C3Vector box1Hs(&(inData->at(2).doubleData[0]));
        C3Vector box2Pos(&(inData->at(3).doubleData[0]));
        C4Vector box2Q(inData->at(4).doubleData[3],inData->at(4).doubleData[0],inData->at(4).doubleData[1],inData->at(4).doubleData[2]);
        C3Vector box2Hs(&(inData->at(5).doubleData[0]));
        bool boxesAreSolid=inData->at(6).boolData[0];
        bool altRoutine=false;
        if ( (inData->size()>7)&&(inData->at(7).boolData.size()==1) )
            altRoutine=inData->at(7).boolData[0];
        C3Vector distPt1,distPt2;
        double dist=geom_getBoxBoxDistance(C7Vector(box1Q,box1Pos),box1Hs,C7Vector(box2Q,box2Pos),box2Hs,boxesAreSolid,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> pp1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp1));
        std::vector<double> pp2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxTriangleDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETBOXTRIANGLEDISTANCE[]={
    8,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
};

void LUA_GETBOXTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXTRIANGLEDISTANCE,inArgs_GETBOXTRIANGLEDISTANCE[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).doubleData[0]));
        C4Vector boxQ(inData->at(1).doubleData[3],inData->at(1).doubleData[0],inData->at(1).doubleData[1],inData->at(1).doubleData[2]);
        C3Vector boxHs(&(inData->at(2).doubleData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector p1(&(inData->at(4).doubleData[0]));
        C3Vector p2(&(inData->at(5).doubleData[0]));
        C3Vector p3(&(inData->at(6).doubleData[0]));
        bool altRoutine=false;
        if ( (inData->size()>7)&&(inData->at(7).boolData.size()==1) )
            altRoutine=inData->at(7).boolData[0];
        C3Vector distPt1,distPt2;
        double dist=geom_getBoxTriangleDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,p1,p2-p1,p3-p1,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> pp1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp1));
        std::vector<double> pp2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxSegmentDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETBOXSEGMENTDISTANCE[]={
    7,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
};

void LUA_GETBOXSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXSEGMENTDISTANCE,inArgs_GETBOXSEGMENTDISTANCE[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).doubleData[0]));
        C4Vector boxQ(inData->at(1).doubleData[3],inData->at(1).doubleData[0],inData->at(1).doubleData[1],inData->at(1).doubleData[2]);
        C3Vector boxHs(&(inData->at(2).doubleData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector segP1(&(inData->at(4).doubleData[0]));
        C3Vector segP2(&(inData->at(5).doubleData[0]));
        bool altRoutine=false;
        if ( (inData->size()>6)&&(inData->at(6).boolData.size()==1) )
            altRoutine=inData->at(6).boolData[0];
        C3Vector distPt1,distPt2;
        double dist=geom_getBoxSegmentDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,segP1,segP2-segP1,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<double> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxPointDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETBOXPOINTDISTANCE[]={
    5,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_double|sim_lua_arg_table,3,
};

void LUA_GETBOXPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXPOINTDISTANCE,inArgs_GETBOXPOINTDISTANCE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).doubleData[0]));
        C4Vector boxQ(inData->at(1).doubleData[3],inData->at(1).doubleData[0],inData->at(1).doubleData[1],inData->at(1).doubleData[2]);
        C3Vector boxHs(&(inData->at(2).doubleData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector point(&(inData->at(4).doubleData[0]));
        C3Vector distPt;
        double dist=geom_getBoxPointDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt.data,distPt.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTriangleTriangleDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETTRIANGLETRIANGLEDISTANCE[]={
    6,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLETRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLETRIANGLEDISTANCE,inArgs_GETTRIANGLETRIANGLEDISTANCE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tpa1(&(inData->at(0).doubleData[0]));
        C3Vector tpa2(&(inData->at(1).doubleData[0]));
        C3Vector tpa3(&(inData->at(2).doubleData[0]));
        C3Vector tpb1(&(inData->at(3).doubleData[0]));
        C3Vector tpb2(&(inData->at(4).doubleData[0]));
        C3Vector tpb3(&(inData->at(5).doubleData[0]));
        C3Vector distPt1,distPt2;
        double dist=geom_getTriangleTriangleDistance(tpa1,tpa2-tpa1,tpa3-tpa1,tpb1,tpb2-tpb1,tpb3-tpb1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<double> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTriangleSegmentDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETTRIANGLESEGMENTDISTANCE[]={
    5,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLESEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLESEGMENTDISTANCE,inArgs_GETTRIANGLESEGMENTDISTANCE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tp1(&(inData->at(0).doubleData[0]));
        C3Vector tp2(&(inData->at(1).doubleData[0]));
        C3Vector tp3(&(inData->at(2).doubleData[0]));
        C3Vector segP1(&(inData->at(3).doubleData[0]));
        C3Vector segP2(&(inData->at(4).doubleData[0]));
        C3Vector distPt1,distPt2;
        double dist=geom_getTriangleSegmentDistance(tp1,tp2-tp1,tp3-tp1,segP1,segP2-segP1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<double> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTrianglePointDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETTRIANGLEPOINTDISTANCE[]={
    4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLEPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLEPOINTDISTANCE,inArgs_GETTRIANGLEPOINTDISTANCE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tp1(&(inData->at(0).doubleData[0]));
        C3Vector tp2(&(inData->at(1).doubleData[0]));
        C3Vector tp3(&(inData->at(2).doubleData[0]));
        C3Vector point(&(inData->at(3).doubleData[0]));
        C3Vector distPt;
        double dist=geom_getTrianglePointDistance(tp1,tp2-tp1,tp3-tp1,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt.data,distPt.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getSegmentSegmentDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETSEGMENTSEGMENTDISTANCE[]={
    4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
};

void LUA_GETSEGMENTSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSEGMENTSEGMENTDISTANCE,inArgs_GETSEGMENTSEGMENTDISTANCE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector segaP1(&(inData->at(0).doubleData[0]));
        C3Vector segaP2(&(inData->at(1).doubleData[0]));
        C3Vector segbP1(&(inData->at(2).doubleData[0]));
        C3Vector segbP2(&(inData->at(3).doubleData[0]));
        C3Vector distPt1,distPt2;
        double dist=geom_getSegmentSegmentDistance(segaP1,segaP2-segaP1,segbP1,segbP2-segbP1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<double> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getSegmentPointDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETSEGMENTPOINTDISTANCE[]={
    3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
};

void LUA_GETSEGMENTPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSEGMENTPOINTDISTANCE,inArgs_GETSEGMENTPOINTDISTANCE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector segPt1(&(inData->at(0).doubleData[0]));
        C3Vector segPt2(&(inData->at(1).doubleData[0]));
        C3Vector point(&(inData->at(3).doubleData[0]));
        C3Vector distPt;
        double dist=geom_getSegmentPointDistance(segPt1,segPt2-segPt1,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<double> p1(distPt.data,distPt.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createMesh
// --------------------------------------------------------------------------------------
const int inArgs_CREATEMESH[]={
    6,
    sim_script_arg_double|sim_lua_arg_table,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0,
    sim_script_arg_int32,0,
};

void LUA_CREATEMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEMESH,inArgs_CREATEMESH[0]-4,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        double maxTriS=double(0.3);
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=3)&&(inData->at(2).doubleData.size()>=3) )
            pos.setData(&inData->at(2).doubleData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()>=4) )
        {
            quat(0)=inData->at(3).doubleData[3];
            quat(1)=inData->at(3).doubleData[0];
            quat(2)=inData->at(3).doubleData[1];
            quat(3)=inData->at(3).doubleData[2];
        }
        C7Vector tr(quat,pos);
        if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1) )
            maxTriS=inData->at(4).doubleData[0];
        int maxTri=8;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
            maxTri=inData->at(5).int32Data[0];
        CObbStruct* obbStruct=geom_createMesh(&inData->at(0).doubleData[0],int(inData->at(0).doubleData.size()),&inData->at(1).int32Data[0],int(inData->at(1).int32Data.size()),&tr,maxTriS,maxTri);
        _meshData[_nextMeshDataHandle]=obbStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextMeshDataHandle));
        _nextMeshDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.destroyMesh
// --------------------------------------------------------------------------------------
const int inArgs_DESTROYMESH[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROYMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROYMESH,inArgs_DESTROYMESH[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            geom_destroyMesh(it->second);
            _meshData.erase(it);
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.copyMesh
// --------------------------------------------------------------------------------------
const int inArgs_COPYMESH[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_COPYMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYMESH,inArgs_COPYMESH[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            CObbStruct* obbStruct=geom_copyMesh(it->second);
            _meshData[_nextMeshDataHandle]=obbStruct;
            D.pushOutData(CScriptFunctionDataItem(_nextMeshDataHandle));
            _nextMeshDataHandle++;
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.scaleMesh
// --------------------------------------------------------------------------------------
const int inArgs_SCALEMESH[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_double,0,
};

void LUA_SCALEMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SCALEMESH,inArgs_SCALEMESH[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
            geom_scaleMesh(it->second,inData->at(1).doubleData[0]);
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshSerializationData
// --------------------------------------------------------------------------------------
const int inArgs_GETMESHSERIALIZATIONDATA[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETMESHSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHSERIALIZATIONDATA,inArgs_GETMESHSERIALIZATIONDATA[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            std::vector<unsigned char> data;
            geom_getMeshSerializationData(it->second,data);
            D.pushOutData(CScriptFunctionDataItem((const char*)&data[0],data.size()));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createMeshFromSerializationData
// --------------------------------------------------------------------------------------
const int inArgs_CREATEMESHFROMSERIALIZATIONDATA[]={
    1,
    sim_script_arg_string,0,
};

void LUA_CREATEMESHFROMSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEMESHFROMSERIALIZATIONDATA,inArgs_CREATEMESHFROMSERIALIZATIONDATA[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::vector<unsigned char> data;
        for (size_t i=0;i<inData->at(0).stringData.size();i++)
            data.push_back((unsigned char)inData->at(0).stringData[0].c_str()[i]);
        CObbStruct* obbStruct=geom_getMeshFromSerializationData(&data[0]);
        _meshData[_nextMeshDataHandle]=obbStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextMeshDataHandle));
        _nextMeshDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromPoints
// --------------------------------------------------------------------------------------
const int inArgs_CREATEOCTREEFROMPOINTS[]={
    6,
    sim_script_arg_double|sim_lua_arg_table,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0,
    sim_script_arg_int32|sim_lua_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_CREATEOCTREEFROMPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMPOINTS,inArgs_CREATEOCTREEFROMPOINTS[0]-5,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).doubleData.size()>=3) )
            pos.setData(&inData->at(1).doubleData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).doubleData.size()>=4) )
        {
            quat(0)=inData->at(2).doubleData[3];
            quat(1)=inData->at(2).doubleData[0];
            quat(2)=inData->at(2).doubleData[1];
            quat(3)=inData->at(2).doubleData[2];
        }
        C7Vector tr(quat,pos);
        double cellS=double(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()==1) )
            cellS=inData->at(3).doubleData[0];
        unsigned char rgb[3]={0,0,0};
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()>=3) )
        {
            rgb[0]=(unsigned char)inData->at(4).int32Data[0];
            rgb[1]=(unsigned char)inData->at(4).int32Data[1];
            rgb[2]=(unsigned char)inData->at(4).int32Data[2];
        }
        int usrdata=0;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
            usrdata=inData->at(5).int32Data[0];

        COcStruct* ocStruct=geom_createOctreeFromPoints(&inData->at(0).doubleData[0],inData->at(0).doubleData.size()/3,&tr,cellS,rgb,(unsigned int)usrdata);
        _octreeData[_nextOctreeDataHandle]=ocStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextOctreeDataHandle));
        _nextOctreeDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromColorPoints
// --------------------------------------------------------------------------------------
const int inArgs_CREATEOCTREEFROMCOLORPOINTS[]={
    6,
    sim_script_arg_double|sim_lua_arg_table,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
};

void LUA_CREATEOCTREEFROMCOLORPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMCOLORPOINTS,inArgs_CREATEOCTREEFROMCOLORPOINTS[0]-5,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).doubleData.size()>=3) )
            pos.setData(&inData->at(1).doubleData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).doubleData.size()>=4) )
        {
            quat(0)=inData->at(2).doubleData[3];
            quat(1)=inData->at(2).doubleData[0];
            quat(2)=inData->at(2).doubleData[1];
            quat(3)=inData->at(2).doubleData[2];
        }
        C7Vector tr(quat,pos);
        double cellS=double(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()==1) )
            cellS=inData->at(3).doubleData[0];
        unsigned char* rgb=nullptr;
        std::vector<unsigned char> _rgb;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()>=inData->at(0).doubleData.size()) )
        {
            _rgb.resize(inData->at(4).int32Data.size());
            for (size_t i=0;i<inData->at(4).int32Data.size();i++)
                _rgb[i]=(unsigned char)inData->at(4).int32Data[i];
            rgb=&_rgb[0];
        }
        unsigned int* usrdata=nullptr;
        std::vector<unsigned int> _usrdata;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=inData->at(0).doubleData.size()/3) )
        {
            _usrdata.resize(inData->at(5).int32Data.size());
            for (size_t i=0;i<inData->at(5).int32Data.size();i++)
                _usrdata[i]=(unsigned int)inData->at(5).int32Data[i];
            usrdata=&_usrdata[0];
        }
        COcStruct* ocStruct=geom_createOctreeFromColorPoints(&inData->at(0).doubleData[0],inData->at(0).doubleData.size()/3,&tr,cellS,rgb,usrdata);
        _octreeData[_nextOctreeDataHandle]=ocStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextOctreeDataHandle));
        _nextOctreeDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromMesh
// --------------------------------------------------------------------------------------
const int inArgs_CREATEOCTREEFROMMESH[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0,
    sim_script_arg_int32|sim_lua_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_CREATEOCTREEFROMMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMMESH,inArgs_CREATEOCTREEFROMMESH[0]-5,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).doubleData[0]));
            C4Vector meshQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector pos;
            pos.clear();
            if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()>=3) )
                pos.setData(&inData->at(3).doubleData[0]);
            C4Vector quat;
            quat.setIdentity();
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()>=4) )
            {
                quat(0)=inData->at(4).doubleData[3];
                quat(1)=inData->at(4).doubleData[0];
                quat(2)=inData->at(4).doubleData[1];
                quat(3)=inData->at(4).doubleData[2];
            }
            C7Vector tr(quat,pos);
            double cellS=double(0.05);
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cellS=inData->at(5).doubleData[0];
            unsigned char rgb[3]={0,0,0};
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()>=3) )
            {
                rgb[0]=(unsigned char)inData->at(6).int32Data[0];
                rgb[1]=(unsigned char)inData->at(6).int32Data[1];
                rgb[2]=(unsigned char)inData->at(6).int32Data[2];
            }
            int usrdata=0;
            if ( (inData->size()>=8)&&(inData->at(7).int32Data.size()==1) )
                usrdata=inData->at(7).int32Data[0];
            COcStruct* ocStruct=geom_createOctreeFromMesh(it->second,C7Vector(meshQ,meshPos),&tr,cellS,rgb,(unsigned int)usrdata);
            _octreeData[_nextOctreeDataHandle]=ocStruct;
            D.pushOutData(CScriptFunctionDataItem(_nextOctreeDataHandle));
            _nextOctreeDataHandle++;
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromOctree
// --------------------------------------------------------------------------------------
const int inArgs_CREATEOCTREEFROMOCTREE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0,
    sim_script_arg_int32|sim_lua_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_CREATEOCTREEFROMOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMOCTREE,inArgs_CREATEOCTREEFROMOCTREE[0]-5,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector ocPos(&(inData->at(1).doubleData[0]));
            C4Vector ocQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector pos;
            pos.clear();
            if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()>=3) )
                pos.setData(&inData->at(3).doubleData[0]);
            C4Vector quat;
            quat.setIdentity();
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()>=4) )
            {
                quat(0)=inData->at(4).doubleData[3];
                quat(1)=inData->at(4).doubleData[0];
                quat(2)=inData->at(4).doubleData[1];
                quat(3)=inData->at(4).doubleData[2];
            }
            C7Vector tr(quat,pos);
            double cellS=double(0.05);
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cellS=inData->at(5).doubleData[0];
            unsigned char rgb[3]={0,0,0};
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()>=3) )
            {
                rgb[0]=(unsigned char)inData->at(6).int32Data[0];
                rgb[1]=(unsigned char)inData->at(6).int32Data[1];
                rgb[2]=(unsigned char)inData->at(6).int32Data[2];
            }
            int usrdata=0;
            if ( (inData->size()>=8)&&(inData->at(7).int32Data.size()==1) )
                usrdata=inData->at(7).int32Data[0];
            COcStruct* ocStruct=geom_createOctreeFromOctree(it->second,C7Vector(ocQ,ocPos),&tr,cellS,rgb,(unsigned int)usrdata);
            _octreeData[_nextOctreeDataHandle]=ocStruct;
            D.pushOutData(CScriptFunctionDataItem(_nextOctreeDataHandle));
            _nextOctreeDataHandle++;
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.destroyOctree
// --------------------------------------------------------------------------------------
const int inArgs_DESTROYOCTREE[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROYOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROYOCTREE,inArgs_DESTROYOCTREE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            geom_destroyOctree(it->second);
            _octreeData.erase(it);
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.copyOctree
// --------------------------------------------------------------------------------------
const int inArgs_COPYOCTREE[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_COPYOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYOCTREE,inArgs_COPYOCTREE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            COcStruct* octreeStruct=geom_copyOctree(it->second);
            _octreeData[_nextOctreeDataHandle]=octreeStruct;
            D.pushOutData(CScriptFunctionDataItem(_nextOctreeDataHandle));
            _nextOctreeDataHandle++;
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.scaleOctree
// --------------------------------------------------------------------------------------
const int inArgs_SCALEOCTREE[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_double,0,
};

void LUA_SCALEOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SCALEOCTREE,inArgs_SCALEOCTREE[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
            geom_scaleOctree(it->second,inData->at(1).doubleData[0]);
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeSerializationData
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREESERIALIZATIONDATA[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETOCTREESERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREESERIALIZATIONDATA,inArgs_GETOCTREESERIALIZATIONDATA[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            std::vector<unsigned char> data;
            geom_getOctreeSerializationData(it->second,data);
            D.pushOutData(CScriptFunctionDataItem((const char*)&data[0],data.size()));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromSerializationData
// --------------------------------------------------------------------------------------
const int inArgs_CREATEOCTREEFROMSERIALIZATIONDATA[]={
    1,
    sim_script_arg_string,0,
};

void LUA_CREATEOCTREEFROMSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMSERIALIZATIONDATA,inArgs_CREATEOCTREEFROMSERIALIZATIONDATA[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::vector<unsigned char> data;
        for (size_t i=0;i<inData->at(0).stringData.size();i++)
            data.push_back((unsigned char)inData->at(0).stringData[0].c_str()[i]);
        COcStruct* octreeStruct=geom_getOctreeFromSerializationData(&data[0]);
        _octreeData[_nextOctreeDataHandle]=octreeStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextOctreeDataHandle));
        _nextOctreeDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeVoxels
// --------------------------------------------------------------------------------------
const int inArgs_GETOCTREEVOXELS[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETOCTREEVOXELS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEVOXELS,inArgs_GETOCTREEVOXELS[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            std::vector<double> data;
            std::vector<unsigned int> usrData;
            geom_getOctreeVoxelData(it->second,data,&usrData);
            std::vector<double> pos;
            std::vector<int> rgb;
            for (size_t i=0;i<data.size()/6;i++)
            {
                pos.push_back(data[6*i+0]);
                pos.push_back(data[6*i+1]);
                pos.push_back(data[6*i+2]);
                rgb.push_back(int(data[6*i+3]*double(255.1)));
                rgb.push_back(int(data[6*i+4]*double(255.1)));
                rgb.push_back(int(data[6*i+5]*double(255.1)));
            }
            D.pushOutData(CScriptFunctionDataItem(pos));
            D.pushOutData(CScriptFunctionDataItem(rgb));
            std::vector<int> _usrData;
            _usrData.resize(usrData.size());
            for (size_t i=0;i<usrData.size();i++)
                _usrData[i]=int(usrData[i]);
            D.pushOutData(CScriptFunctionDataItem(_usrData));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTransformedPoints
// --------------------------------------------------------------------------------------
const int inArgs_GETTRANSFORMEDPOINTS[]={
    3,
    sim_script_arg_double|sim_lua_arg_table,0, // points
    sim_script_arg_double|sim_lua_arg_table,3, // position (3 vals) or matrix (12 vals)
    sim_script_arg_double|sim_lua_arg_table,3, // euler (3 vals) or quaternion (4 vals)
};

void LUA_GETTRANSFORMEDPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRANSFORMEDPOINTS,inArgs_GETTRANSFORMEDPOINTS[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::vector<double> outData;
        outData.resize(inData->at(0).doubleData.size());
        C7Vector tr;
        if (inData->at(1).doubleData.size()<12)
        { // second argument is a position
            C3Vector pos(&(inData->at(1).doubleData[0]));
            if (inData->at(2).doubleData.size()<4)
            { // third argument is a Euler angle value
                C3Vector e(&inData->at(2).doubleData[0]);
                tr.X=pos;
                tr.Q.setEulerAngles(e);
            }
            else
            { // third argument is a quaternion
                C4Vector q(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
                tr=C7Vector(q,pos);
            }
        }
        else
        { // second argument is a matrix
            C4X4Matrix m;
            m.setData(&(inData->at(1).doubleData[0]));
            tr=m.getTransformation();
        }
        for (size_t i=0;i<inData->at(0).doubleData.size()/3;i++)
        {
            C3Vector v(&inData->at(0).doubleData[0]+3*i);
            v*=tr;
            outData[3*i+0]=v(0);
            outData[3*i+1]=v(1);
            outData[3*i+2]=v(2);
        }
        D.pushOutData(CScriptFunctionDataItem(outData));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// simGeom.createPtcloudFromPoints
// --------------------------------------------------------------------------------------
const int inArgs_CREATEPTCLOUDFROMPOINTS[]={
    7,
    sim_script_arg_double|sim_lua_arg_table,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // cell size
    sim_script_arg_int32,0, // max points in cell
    sim_script_arg_int32|sim_lua_arg_table,3, // color
    sim_script_arg_double,0, // proximity tolerance
};

void LUA_CREATEPTCLOUDFROMPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMPOINTS,inArgs_CREATEPTCLOUDFROMPOINTS[0]-6,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).doubleData.size()>=3) )
            pos.setData(&inData->at(1).doubleData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).doubleData.size()>=4) )
        {
            quat(0)=inData->at(2).doubleData[3];
            quat(1)=inData->at(2).doubleData[0];
            quat(2)=inData->at(2).doubleData[1];
            quat(3)=inData->at(2).doubleData[2];
        }
        C7Vector tr(quat,pos);
        double cellS=double(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()==1) )
            cellS=inData->at(3).doubleData[0];
        int maxPts=20;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()==1) )
            maxPts=inData->at(4).int32Data[0];
        unsigned char rgb[3]={0,0,0};
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=3) )
        {
            rgb[0]=(unsigned char)inData->at(5).int32Data[0];
            rgb[1]=(unsigned char)inData->at(5).int32Data[1];
            rgb[2]=(unsigned char)inData->at(5).int32Data[2];
        }
        double proxTol=double(0.005);
        if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
            proxTol=inData->at(6).doubleData[0];
        CPcStruct* pcStruct=geom_createPtcloudFromPoints(&inData->at(0).doubleData[0],inData->at(0).doubleData.size()/3,&tr,cellS,maxPts,rgb,proxTol);
        _ptcloudData[_nextPtcloudDataHandle]=pcStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextPtcloudDataHandle));
        _nextPtcloudDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createPtcloudFromColorPoints
// --------------------------------------------------------------------------------------
const int inArgs_CREATEPTCLOUDFROMCOLORPOINTS[]={
    7,
    sim_script_arg_double|sim_lua_arg_table,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // cell size
    sim_script_arg_int32,0, // max points in cell
    sim_script_arg_int32|sim_lua_arg_table,0, // color
    sim_script_arg_double,0, // proximity tolerance
};

void LUA_CREATEPTCLOUDFROMCOLORPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMCOLORPOINTS,inArgs_CREATEPTCLOUDFROMCOLORPOINTS[0]-5,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).doubleData.size()>=3) )
            pos.setData(&inData->at(1).doubleData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).doubleData.size()>=4) )
        {
            quat(0)=inData->at(2).doubleData[3];
            quat(1)=inData->at(2).doubleData[0];
            quat(2)=inData->at(2).doubleData[1];
            quat(3)=inData->at(2).doubleData[2];
        }
        C7Vector tr(quat,pos);
        double cellS=double(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).doubleData.size()==1) )
            cellS=inData->at(3).doubleData[0];
        int maxPts=20;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()==1) )
            maxPts=inData->at(4).int32Data[0];
        unsigned char* rgb=nullptr;
        std::vector<unsigned char> _rgb;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=inData->at(0).doubleData.size()) )
        {
            _rgb.resize(inData->at(5).int32Data.size());
            for (size_t i=0;i<inData->at(5).int32Data.size();i++)
                _rgb[i]=(unsigned char)inData->at(5).int32Data[i];
            rgb=&_rgb[0];
        }
        double proxTol=double(0.005);
        if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
            proxTol=inData->at(6).doubleData[0];
        CPcStruct* pcStruct=geom_createPtcloudFromColorPoints(&inData->at(0).doubleData[0],inData->at(0).doubleData.size()/3,&tr,cellS,maxPts,rgb,proxTol);
        _ptcloudData[_nextPtcloudDataHandle]=pcStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextPtcloudDataHandle));
        _nextPtcloudDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.destroyPtcloud
// --------------------------------------------------------------------------------------
const int inArgs_DESTROYPTCLOUD[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROYPTCLOUD_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROYPTCLOUD,inArgs_DESTROYPTCLOUD[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            geom_destroyPtcloud(it->second);
            _ptcloudData.erase(it);
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.copyPtcloud
// --------------------------------------------------------------------------------------
const int inArgs_COPYPTCLOUD[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_COPYPTCLOUD_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYPTCLOUD,inArgs_COPYPTCLOUD[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            CPcStruct* ptcloudStruct=geom_copyPtcloud(it->second);
            _ptcloudData[_nextPtcloudDataHandle]=ptcloudStruct;
            D.pushOutData(CScriptFunctionDataItem(_nextPtcloudDataHandle));
            _nextPtcloudDataHandle++;
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.scalePtcloud
// --------------------------------------------------------------------------------------
const int inArgs_SCALEPTCLOUD[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_double,0,
};

void LUA_SCALEPTCLOUD_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SCALEPTCLOUD,inArgs_SCALEPTCLOUD[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
            geom_scalePtcloud(it->second,inData->at(1).doubleData[0]);
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudSerializationData
// --------------------------------------------------------------------------------------
const int inArgs_GETPTCLOUDSERIALIZATIONDATA[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETPTCLOUDSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDSERIALIZATIONDATA,inArgs_GETPTCLOUDSERIALIZATIONDATA[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            std::vector<unsigned char> data;
            geom_getPtcloudSerializationData(it->second,data);
            D.pushOutData(CScriptFunctionDataItem((const char*)&data[0],data.size()));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createPtcloudFromSerializationData
// --------------------------------------------------------------------------------------
const int inArgs_CREATEPTCLOUDFROMSERIALIZATIONDATA[]={
    1,
    sim_script_arg_string,0,
};

void LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMSERIALIZATIONDATA,inArgs_CREATEPTCLOUDFROMSERIALIZATIONDATA[0],nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::vector<unsigned char> data;
        for (size_t i=0;i<inData->at(0).stringData.size();i++)
            data.push_back((unsigned char)inData->at(0).stringData[0].c_str()[i]);
        CPcStruct* ptcloudStruct=geom_getPtcloudFromSerializationData(&data[0]);
        _ptcloudData[_nextPtcloudDataHandle]=ptcloudStruct;
        D.pushOutData(CScriptFunctionDataItem(_nextPtcloudDataHandle));
        _nextPtcloudDataHandle++;
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudPoints
// --------------------------------------------------------------------------------------
const int inArgs_GETPTCLOUDPOINTS[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_double,0,
};

void LUA_GETPTCLOUDPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDPOINTS,inArgs_GETPTCLOUDPOINTS[0]-1,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            double prop=1.0;
            if ( (inData->size()>=2)&&(inData->at(1).doubleData.size()==1) )
                prop=inData->at(1).doubleData[0];
            std::vector<double> data;
            geom_getPtcloudPoints(it->second,data,prop);
            std::vector<double> pos;
            std::vector<int> rgb;
            for (size_t i=0;i<data.size()/6;i++)
            {
                pos.push_back(data[6*i+0]);
                pos.push_back(data[6*i+1]);
                pos.push_back(data[6*i+2]);
                rgb.push_back(int(data[6*i+3]*double(255.1)));
                rgb.push_back(int(data[6*i+4]*double(255.1)));
                rgb.push_back(int(data[6*i+5]*double(255.1)));
            }
            D.pushOutData(CScriptFunctionDataItem(pos));
            D.pushOutData(CScriptFunctionDataItem(rgb));
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudPtcloudDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETPTCLOUDPTCLOUDDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETPTCLOUDPTCLOUDDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDPTCLOUDDISTANCE,inArgs_GETPTCLOUDPTCLOUDDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CPcStruct*>::iterator it1=_ptcloudData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_ptcloudData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector ptcloud1Pos(&(inData->at(1).doubleData[0]));
            C4Vector ptcloud1Q(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector ptcloud2Pos(&(inData->at(4).doubleData[0]));
            C4Vector ptcloud2Q(inData->at(5).doubleData[3],inData->at(5).doubleData[0],inData->at(5).doubleData[1],inData->at(5).doubleData[2]);
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=(unsigned long long int)inData->at(7).doubleData[0];
                cache2=(unsigned long long int)inData->at(7).doubleData[1];
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudPtcloudDistanceIfSmaller(it1->second,C7Vector(ptcloud1Q,ptcloud1Pos),it2->second,C7Vector(ptcloud2Q,ptcloud2Pos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudTriangleDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETPTCLOUDTRIANGLEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETPTCLOUDTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDTRIANGLEDISTANCE,inArgs_GETPTCLOUDTRIANGLEDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            C3Vector ptcloudPos(&(inData->at(1).doubleData[0]));
            C4Vector ptcloudQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector triPt1(&(inData->at(3).doubleData[0]));
            C3Vector triPt2(&(inData->at(4).doubleData[0]));
            C3Vector triPt3(&(inData->at(5).doubleData[0]));
            double dist=DBL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1)&&(inData->at(6).doubleData[0]!=0.0) )
                dist=inData->at(6).doubleData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(7).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudTriangleDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudSegmentDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETPTCLOUDSEGMENTDISTANCE[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETPTCLOUDSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDSEGMENTDISTANCE,inArgs_GETPTCLOUDSEGMENTDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            C3Vector ptcloudPos(&(inData->at(1).doubleData[0]));
            C4Vector ptcloudQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector segPt1(&(inData->at(3).doubleData[0]));
            C3Vector segPt2(&(inData->at(4).doubleData[0]));
            double dist=DBL_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1)&&(inData->at(5).doubleData[0]!=0.0) )
                dist=inData->at(5).doubleData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(6).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudSegmentDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<double> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudPointDistance
// --------------------------------------------------------------------------------------
const int inArgs_GETPTCLOUDPOINTDISTANCE[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,3,
    sim_script_arg_double,0, // threshold, default is DBL_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETPTCLOUDPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDPOINTDISTANCE,inArgs_GETPTCLOUDPOINTDISTANCE[0]-2,nullptr))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            C3Vector ptcloudPos(&(inData->at(1).doubleData[0]));
            C4Vector ptcloudQ(inData->at(2).doubleData[3],inData->at(2).doubleData[0],inData->at(2).doubleData[1],inData->at(2).doubleData[2]);
            C3Vector point(&(inData->at(3).doubleData[0]));
            double dist=DBL_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1)&&(inData->at(4).doubleData[0]!=0.0) )
                dist=inData->at(4).doubleData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(5).doubleData[0];
            C3Vector distPt;
            bool smaller=geom_getPtcloudPointDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),point,dist,&distPt,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<double> pp1(distPt.data,distPt.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(nullptr,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

SIM_DLLEXPORT int simInit(SSimInit* info)
{
    simLib=loadSimLibrary(info->coppeliaSimLibPath);
    if (simLib==NULL)
    {
        simAddLog(info->pluginName,sim_verbosity_errors,"could not find or correctly load the CoppeliaSim library. Cannot start the plugin.");
        return(0);
    }
    if (getSimProcAddresses(simLib)==0)
    {
        simAddLog(info->pluginName,sim_verbosity_errors,"could not find all required functions in the CoppeliaSim library. Cannot start the plugin.");
        unloadSimLibrary(simLib);
        return(0);
    }

    // Register the new Lua commands:
    simRegisterScriptCallbackFunction("getMeshMeshCollision",nullptr,LUA_GETMESHMESHCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshOctreeCollision",nullptr,LUA_GETMESHOCTREECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshTriangleCollision",nullptr,LUA_GETMESHTRIANGLECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshSegmentCollision",nullptr,LUA_GETMESHSEGMENTCOLLISION_CALLBACK);

    simRegisterScriptCallbackFunction("getOctreeOctreeCollision",nullptr,LUA_GETOCTREEOCTREECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreePtcloudCollision",nullptr,LUA_GETOCTREEPTCLOUDCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreeTriangleCollision",nullptr,LUA_GETOCTREETRIANGLECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreeSegmentCollision",nullptr,LUA_GETOCTREESEGMENTCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreePointCollision",nullptr,LUA_GETOCTREEPOINTCOLLISION_CALLBACK);

    simRegisterScriptCallbackFunction("getMeshMeshDistance",nullptr,LUA_GETMESHMESHDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshOctreeDistance",nullptr,LUA_GETMESHOCTREEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshPtcloudDistance",nullptr,LUA_GETMESHPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshTriangleDistance",nullptr,LUA_GETMESHTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshSegmentDistance",nullptr,LUA_GETMESHSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshPointDistance",nullptr,LUA_GETMESHPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction("getOctreeOctreeDistance",nullptr,LUA_GETOCTREEOCTREEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreePtcloudDistance",nullptr,LUA_GETOCTREEPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreeTriangleDistance",nullptr,LUA_GETOCTREETRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreeSegmentDistance",nullptr,LUA_GETOCTREESEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreePointDistance",nullptr,LUA_GETOCTREEPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction("getPtcloudPtcloudDistance",nullptr,LUA_GETPTCLOUDPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getPtcloudTriangleDistance",nullptr,LUA_GETPTCLOUDTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getPtcloudSegmentDistance",nullptr,LUA_GETPTCLOUDSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getPtcloudPointDistance",nullptr,LUA_GETPTCLOUDPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction("getBoxBoxDistance",nullptr,LUA_GETBOXBOXDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getBoxTriangleDistance",nullptr,LUA_GETBOXTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getBoxSegmentDistance",nullptr,LUA_GETBOXSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getBoxPointDistance",nullptr,LUA_GETBOXPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction("getTriangleTriangleDistance",nullptr,LUA_GETTRIANGLETRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getTriangleSegmentDistance",nullptr,LUA_GETTRIANGLESEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getTrianglePointDistance",nullptr,LUA_GETTRIANGLEPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction("getSegmentSegmentDistance",nullptr,LUA_GETSEGMENTSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction("getSegmentPointDistance",nullptr,LUA_GETSEGMENTPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction("createMesh",nullptr,LUA_CREATEMESH_CALLBACK);
    simRegisterScriptCallbackFunction("copyMesh",nullptr,LUA_COPYMESH_CALLBACK);
    simRegisterScriptCallbackFunction("scaleMesh",nullptr,LUA_SCALEMESH_CALLBACK);
    simRegisterScriptCallbackFunction("getMeshSerializationData",nullptr,LUA_GETMESHSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction("createMeshFromSerializationData",nullptr,LUA_CREATEMESHFROMSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction("destroyMesh",nullptr,LUA_DESTROYMESH_CALLBACK);

    simRegisterScriptCallbackFunction("createOctreeFromPoints",nullptr,LUA_CREATEOCTREEFROMPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction("createOctreeFromColorPoints",nullptr,LUA_CREATEOCTREEFROMCOLORPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction("createOctreeFromMesh",nullptr,LUA_CREATEOCTREEFROMMESH_CALLBACK);
    simRegisterScriptCallbackFunction("createOctreeFromOctree",nullptr,LUA_CREATEOCTREEFROMOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction("copyOctree",nullptr,LUA_COPYOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction("scaleOctree",nullptr,LUA_SCALEOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreeSerializationData",nullptr,LUA_GETOCTREESERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction("createOctreeFromSerializationData",nullptr,LUA_CREATEOCTREEFROMSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction("destroyOctree",nullptr,LUA_DESTROYOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction("getOctreeVoxels",nullptr,LUA_GETOCTREEVOXELS_CALLBACK);

    simRegisterScriptCallbackFunction("createPtcloudFromPoints",nullptr,LUA_CREATEPTCLOUDFROMPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction("createPtcloudFromColorPoints",nullptr,LUA_CREATEPTCLOUDFROMCOLORPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction("copyPtcloud",nullptr,LUA_COPYPTCLOUD_CALLBACK);
    simRegisterScriptCallbackFunction("scalePtcloud",nullptr,LUA_SCALEPTCLOUD_CALLBACK);
    simRegisterScriptCallbackFunction("getPtcloudSerializationData",nullptr,LUA_GETPTCLOUDSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction("createPtcloudFromSerializationData",nullptr,LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction("destroyPtcloud",nullptr,LUA_DESTROYPTCLOUD_CALLBACK);
    simRegisterScriptCallbackFunction("getPtcloudPoints",nullptr,LUA_GETPTCLOUDPOINTS_CALLBACK);

    simRegisterScriptCallbackFunction("getTransformedPoints",nullptr,LUA_GETTRANSFORMEDPOINTS_CALLBACK);

    return(5);
}

SIM_DLLEXPORT void simCleanup()
{
}

SIM_DLLEXPORT void simMsg(SSimMsg* info)
{
    if (info->msgId==sim_message_eventcallback_lastinstancepass)
    {
        for (std::map<int,CObbStruct*>::iterator it=_meshData.begin();it!=_meshData.end();it++)
            delete it->second;
        _meshData.clear();
        for (std::map<int,COcStruct*>::iterator it=_octreeData.begin();it!=_octreeData.end();it++)
            delete it->second;
        _octreeData.clear();
        for (std::map<int,CPcStruct*>::iterator it=_ptcloudData.begin();it!=_ptcloudData.end();it++)
            delete it->second;
        _ptcloudData.clear();
    }
}

SIM_DLLEXPORT void geomPlugin_releaseBuffer(void* buff)
{
    delete[] ((char*)buff);
}

SIM_DLLEXPORT void* geomPlugin_createMesh(const double* vertices,int verticesSize,const int* indices,int indicesSize,const double meshOrigin[7],double triangleEdgeMaxLength,int maxTrianglesInBoundingBox)
{
    C7Vector tr;
    tr.setIdentity();
    if (meshOrigin!=nullptr)
        tr.setData(meshOrigin);
    void* retVal=geom_createMesh(vertices,verticesSize,indices,indicesSize,&tr,triangleEdgeMaxLength,maxTrianglesInBoundingBox);
    return(retVal);
}

SIM_DLLEXPORT void* geomPlugin_copyMesh(const void* meshObbStruct)
{
    void* retVal=geom_copyMesh((const CObbStruct*)meshObbStruct);
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_scaleMesh(void* meshObbStruct,double scalingFactor)
{
    geom_scaleMesh((CObbStruct*)meshObbStruct,scalingFactor);
}

SIM_DLLEXPORT unsigned char* geomPlugin_getMeshSerializationData(const void* meshObbStruct,int* serializationDataSize)
{
    std::vector<unsigned char> serData;
    geom_getMeshSerializationData((const CObbStruct*)meshObbStruct,serData);
    unsigned char* retVal=new unsigned char[serData.size()];
    for (size_t i=0;i<serData.size();i++)
        retVal[i]=serData[i];
    serializationDataSize[0]=(int)serData.size();
    return(retVal);
}

SIM_DLLEXPORT void* geomPlugin_getMeshFromSerializationData(const unsigned char* serializationData)
{
    void* retVal=geom_getMeshFromSerializationData(serializationData);
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_destroyMesh(void* meshObbStruct)
{
    geom_destroyMesh((CObbStruct*)meshObbStruct);
}

SIM_DLLEXPORT double geomPlugin_getMeshRootObbVolume(const void* meshObbStruct)
{
    return(geom_getMeshRootObbVolume((const CObbStruct*)meshObbStruct));
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const double* points,int pointCnt,const double octreeOrigin[7],double cellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromPoints(points,pointCnt,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const double* points,int pointCnt,const double octreeOrigin[7],double cellS,const unsigned char* rgbData,const unsigned int* usrData)
{
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromColorPoints(points,pointCnt,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const void* meshObbStruct,const double meshTransformation[7],const double octreeOrigin[7],double cellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector _meshTransformation;
    _meshTransformation.setData(meshTransformation);
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromMesh((const CObbStruct*)meshObbStruct,_meshTransformation,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const void* otherOctreeStruct,const double otherOctreeTransformation[7],const double newOctreeOrigin[7],double newOctreeCellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector _otherOctreeTransformation;
    _otherOctreeTransformation.setData(otherOctreeTransformation);
    C7Vector tr;
    tr.setIdentity();
    if (newOctreeOrigin!=nullptr)
        tr.setData(newOctreeOrigin);
    COcStruct* retVal=geom_createOctreeFromOctree((const COcStruct*)otherOctreeStruct,_otherOctreeTransformation,&tr,newOctreeCellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_copyOctree(const void* ocStruct)
{
    COcStruct* retVal=geom_copyOctree((const COcStruct*)ocStruct);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData(const unsigned char* serializationData)
{
    COcStruct* retVal=geom_getOctreeFromSerializationData(serializationData);
    return(retVal);
}
SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData(const void* ocStruct,int* serializationDataSize)
{
    std::vector<unsigned char> serData;
    geom_getOctreeSerializationData((const COcStruct*)ocStruct,serData);
    unsigned char* retVal=new unsigned char[serData.size()];
    for (size_t i=0;i<serData.size();i++)
        retVal[i]=serData[i];
    serializationDataSize[0]=(int)serData.size();
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData_float(const unsigned char* serializationData)
{
    COcStruct* retVal=geom_getOctreeFromSerializationData_float(serializationData);
    return(retVal);
}
SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData_float(const void* ocStruct,int* serializationDataSize)
{
    std::vector<unsigned char> serData;
    geom_getOctreeSerializationData_float((const COcStruct*)ocStruct,serData);
    unsigned char* retVal=new unsigned char[serData.size()];
    for (size_t i=0;i<serData.size();i++)
        retVal[i]=serData[i];
    serializationDataSize[0]=(int)serData.size();
    return(retVal);
}
SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,double f)
{
    geom_scaleOctree((COcStruct*)ocStruct,f);
}
SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct)
{
    geom_destroyOctree((COcStruct*)ocStruct);
}
SIM_DLLEXPORT double* geomPlugin_getOctreeVoxelData(const void* ocStruct,int* voxelCount)
{ // voxel position + rgb
    std::vector<double> data;
    geom_getOctreeVoxelData((const COcStruct*)ocStruct,data,nullptr);
    double* retVal=new double[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    voxelCount[0]=((int)data.size())/6;
    return(retVal);
}
SIM_DLLEXPORT unsigned int* geomPlugin_getOctreeUserData(const void* ocStruct,int* voxelCount)
{
    std::vector<double> data;
    std::vector<unsigned int> usrData;
    geom_getOctreeVoxelData((const COcStruct*)ocStruct,data,&usrData);
    unsigned int* retVal=new unsigned int[usrData.size()];
    for (size_t i=0;i<usrData.size();i++)
        retVal[i]=usrData[i];
    voxelCount[0]=(int)usrData.size();
    return(retVal);
}
SIM_DLLEXPORT double* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* pointCount)
{
    std::vector<double> data;
    geom_getOctreeCornersFromOctree((const COcStruct*)ocStruct,data);
    double* retVal=new double[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/3;
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const double octreeTransformation[7],const double* points,int pointCnt,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    geom_insertPointsIntoOctree((COcStruct*)ocStruct,tr,points,pointCnt,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const double octreeTransformation[7],const double* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    geom_insertColorPointsIntoOctree((COcStruct*)ocStruct,tr,points,pointCnt,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const double octreeTransformation[7],const void* obbStruct,const double meshTransformation[7],const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    C7Vector meshTr;
    meshTr.setData(meshTransformation);
    geom_insertMeshIntoOctree((COcStruct*)ocStruct,tr,(const CObbStruct*)obbStruct,meshTr,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7],const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr1;
    tr1.setData(octree1Transformation);
    C7Vector tr2;
    tr2.setData(octree2Transformation);
    geom_insertOctreeIntoOctree((COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,rgbData,usrData);
}
SIM_DLLEXPORT bool geomPlugin_removePointsFromOctree(void* ocStruct,const double octreeTransformation[7],const double* points,int pointCnt)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    bool retVal=geom_removePointsFromOctree((COcStruct*)ocStruct,tr,points,pointCnt);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeMeshFromOctree(void* ocStruct,const double octreeTransformation[7],const void* obbStruct,const double meshTransformation[7])
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    C7Vector meshTr;
    meshTr.setData(meshTransformation);
    bool retVal=geom_removeMeshFromOctree((COcStruct*)ocStruct,tr,(const CObbStruct*)obbStruct,meshTr);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromOctree(void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7])
{
    C7Vector tr1;
    tr1.setData(octree1Transformation);
    C7Vector tr2;
    tr2.setData(octree2Transformation);
    bool retVal=geom_removeOctreeFromOctree((COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2);
    return(retVal);
}



SIM_DLLEXPORT void* geomPlugin_createPtcloudFromPoints(const double* points,int pointCnt,const double ptcloudOrigin[7],double cellS,int maxPointCnt,const unsigned char rgbData[3],double proximityTol)
{
    C7Vector tr;
    tr.setIdentity();
    if (ptcloudOrigin!=nullptr)
        tr.setData(ptcloudOrigin);
    void* retVal=geom_createPtcloudFromPoints(points,pointCnt,&tr,cellS,maxPointCnt,rgbData,proximityTol);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromColorPoints(const double* points,int pointCnt,const double ptcloudOrigin[7],double cellS,int maxPointCnt,const unsigned char* rgbData,double proximityTol)
{
    C7Vector tr;
    tr.setIdentity();
    if (ptcloudOrigin!=nullptr)
        tr.setData(ptcloudOrigin);
    void* retVal=geom_createPtcloudFromColorPoints(points,pointCnt,&tr,cellS,maxPointCnt,rgbData,proximityTol);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_copyPtcloud(const void* pcStruct)
{
    void* retVal=geom_copyPtcloud((const CPcStruct*)pcStruct);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_getPtcloudFromSerializationData(const unsigned char* serializationData)
{
    void* retVal=geom_getPtcloudFromSerializationData(serializationData);
    return(retVal);
}
SIM_DLLEXPORT unsigned char* geomPlugin_getPtcloudSerializationData(const void* pcStruct,int* serializationDataSize)
{
    std::vector<unsigned char> serData;
    geom_getPtcloudSerializationData((const CPcStruct*)pcStruct,serData);
    unsigned char* retVal=new unsigned char[serData.size()];
    for (size_t i=0;i<serData.size();i++)
        retVal[i]=serData[i];
    serializationDataSize[0]=(int)serData.size();
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_getPtcloudFromSerializationData_float(const unsigned char* serializationData)
{
    void* retVal=geom_getPtcloudFromSerializationData_float(serializationData);
    return(retVal);
}
SIM_DLLEXPORT unsigned char* geomPlugin_getPtcloudSerializationData_float(const void* pcStruct,int* serializationDataSize)
{
    std::vector<unsigned char> serData;
    geom_getPtcloudSerializationData_float((const CPcStruct*)pcStruct,serData);
    unsigned char* retVal=new unsigned char[serData.size()];
    for (size_t i=0;i<serData.size();i++)
        retVal[i]=serData[i];
    serializationDataSize[0]=(int)serData.size();
    return(retVal);
}
SIM_DLLEXPORT void geomPlugin_scalePtcloud(void* pcStruct,double f)
{
    geom_scalePtcloud((CPcStruct*)pcStruct,f);
}
SIM_DLLEXPORT void geomPlugin_destroyPtcloud(void* pcStruct)
{
    geom_destroyPtcloud((CPcStruct*)pcStruct);
}
SIM_DLLEXPORT double* geomPlugin_getPtcloudPoints(const void* pcStruct,int* pointCount,double prop)
{
    std::vector<double> data;
    geom_getPtcloudPoints((const CPcStruct*)pcStruct,data,prop);
    double* retVal=new double[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/6;
    return(retVal);
}
SIM_DLLEXPORT double* geomPlugin_getPtcloudOctreeCorners(const void* pcStruct,int* pointCount)
{
    std::vector<double> data;
    geom_getPtcloudOctreeCorners((const CPcStruct*)pcStruct,data);
    double* retVal=new double[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/3;
    return(retVal);
}
SIM_DLLEXPORT int geomPlugin_getPtcloudNonEmptyCellCount(const void* pcStruct)
{
    int retVal=geom_getPtcloudNonEmptyCellCount((const CPcStruct*)pcStruct);
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,const unsigned char rgbData[3],double proximityTol)
{
    C7Vector tr;
    tr.setData(ptcloudTransformation);
    geom_insertPointsIntoPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,rgbData,proximityTol);
}
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,const unsigned char* rgbData,double proximityTol)
{
    C7Vector tr;
    tr.setData(ptcloudTransformation);
    geom_insertColorPointsIntoPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,rgbData,proximityTol);
}
SIM_DLLEXPORT bool geomPlugin_removePointsFromPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,double proximityTol,int* countRemoved)
{
    C7Vector tr;
    tr.setData(ptcloudTransformation);
    bool retVal=geom_removePointsFromPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,proximityTol,countRemoved);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromPtcloud(void* pcStruct,const double ptcloudTransformation[7],const void* ocStruct,const double octreeTransformation[7],int* countRemoved)
{
    C7Vector tr;
    tr.setData(ptcloudTransformation);
    C7Vector octreeTr;
    octreeTr.setData(octreeTransformation);
    bool retVal=geom_removeOctreeFromPtcloud((CPcStruct*)pcStruct,tr,(const COcStruct*)ocStruct,octreeTr,countRemoved);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_intersectPointsWithPtcloud(void* pcStruct,const double ptcloudTransformation[7],const double* points,int pointCnt,double proximityTol)
{
    C7Vector tr;
    tr.setData(ptcloudTransformation);
    bool retVal=geom_intersectPointsWithPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,proximityTol);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getMeshMeshCollision(const void* mesh1ObbStruct,const double mesh1Transformation[7],const void* mesh2ObbStruct,const double mesh2Transformation[7],double** intersections,int* intersectionsSize,int* mesh1Caching,int* mesh2Caching)
{
    C7Vector tr1;
    tr1.setData(mesh1Transformation);
    C7Vector tr2;
    tr2.setData(mesh2Transformation);
    std::vector<double> _intersections;
    std::vector<double>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshMeshCollision((const CObbStruct*)mesh1ObbStruct,tr1,(const CObbStruct*)mesh2ObbStruct,tr2,_ints,mesh1Caching,mesh2Caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new double[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeCollision(const void* meshObbStruct,const double meshTransformation[7],const void* ocStruct,const double octreeTransformation[7],int* meshCaching,unsigned long long int* ocCaching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C7Vector tr2;
    tr2.setData(octreeTransformation);
    bool retVal=geom_getMeshOctreeCollision((const CObbStruct*)meshObbStruct,tr1,(const COcStruct*)ocStruct,tr2,meshCaching,ocCaching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleCollision(const void* meshObbStruct,const double meshTransformation[7],const double p[3],const double v[3],const double w[3],double** intersections,int* intersectionsSize,int* caching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    std::vector<double> _intersections;
    std::vector<double>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshTriangleCollision((const CObbStruct*)meshObbStruct,tr1,_p,_v,_w,_ints,caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new double[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentCollision(const void* meshObbStruct,const double meshTransformation[7],const double segmentExtremity[3],const double segmentVector[3],double** intersections,int* intersectionsSize,int* caching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C3Vector _segmentExtremity(segmentExtremity);
    C3Vector _segmentVector(segmentVector);
    std::vector<double> _intersections;
    std::vector<double>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshSegmentCollision((const CObbStruct*)meshObbStruct,tr1,_segmentExtremity,_segmentVector,_ints,caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new double[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching)
{
    C7Vector tr1;
    tr1.setData(octree1Transformation);
    C7Vector tr2;
    tr2.setData(octree2Transformation);
    bool retVal=geom_getOctreeOctreeCollision((const COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,oc1Caching,oc2Caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudCollision(const void* ocStruct,const double octreeTransformation[7],const void* pcStruct,const double ptcloudTransformation[7],unsigned long long int* ocCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setData(octreeTransformation);
    C7Vector tr2;
    tr2.setData(ptcloudTransformation);
    bool retVal=geom_getOctreePtcloudCollision((const COcStruct*)ocStruct,tr1,(const CPcStruct*)pcStruct,tr2,ocCaching,pcCaching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleCollision(const void* ocStruct,const double octreeTransformation[7],const double p[3],const double v[3],const double w[3],unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setData(octreeTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    bool retVal=geom_getOctreeTriangleCollision((const COcStruct*)ocStruct,tr1,_p,_v,_w,caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentCollision(const void* ocStruct,const double octreeTransformation[7],const double segmentExtremity[3],const double segmentVector[3],unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setData(octreeTransformation);
    C3Vector _segmentExtremity(segmentExtremity);
    C3Vector _segmentVector(segmentVector);
    bool retVal=geom_getOctreeSegmentCollision((const COcStruct*)ocStruct,tr1,_segmentExtremity,_segmentVector,caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePointsCollision(const void* ocStruct,const double octreeTransformation[7],const double* points,int pointCount)
{
    C7Vector tr1;
    tr1.setData(octreeTransformation);
    bool retVal=geom_getOctreePointsCollision((const COcStruct*)ocStruct,tr1,points,pointCount);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreePointCollision(const void* ocStruct,const double octreeTransformation[7],const double point[3],unsigned int* usrData,unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setData(octreeTransformation);
    C3Vector _point(point);
    bool retVal=geom_getOctreePointCollision((const COcStruct*)ocStruct,tr1,_point,usrData,caching);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getBoxBoxCollision(const double box1Transformation[7],const double box1HalfSize[3],const double box2Transformation[7],const double box2HalfSize[3],bool boxesAreSolid)
{
    C7Vector tr1;
    tr1.setData(box1Transformation);
    C7Vector tr2;
    tr2.setData(box2Transformation);
    C3Vector b1hs(box1HalfSize);
    C3Vector b2hs(box2HalfSize);
    bool retVal=geom_getBoxBoxCollision(tr1,b1hs,tr2,b2hs,boxesAreSolid);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleCollision(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double p[3],const double v[3],const double w[3])
{
    C7Vector tr;
    tr.setData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    bool retVal=geom_getBoxTriangleCollision(tr,bhs,boxIsSolid,_p,_v,_w);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentCollision(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double segmentEndPoint[3],const double segmentVector[3])
{
    C7Vector tr;
    tr.setData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    bool retVal=geom_getBoxSegmentCollision(tr,bhs,boxIsSolid,_segmentEndPoint,_segmentVector);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxPointCollision(const double boxTransformation[7],const double boxHalfSize[3],const double point[3])
{
    C7Vector tr;
    tr.setData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _point(point);
    bool retVal=geom_getBoxPointCollision(tr,bhs,_point);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleCollision(const double p1[3],const double v1[3],const double w1[3],const double p2[3],const double v2[3],const double w2[3],double** intersections,int* intersectionsSize)
{
    C3Vector _p1(p1);
    C3Vector _v1(v1);
    C3Vector _w1(w1);
    C3Vector _p2(p2);
    C3Vector _v2(v2);
    C3Vector _w2(w2);
    std::vector<double> _intersections;
    std::vector<double>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getTriangleTriangleCollision(_p1,_v1,_w1,_p2,_v2,_w2,_ints);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new double[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentCollision(const double p[3],const double v[3],const double w[3],const double segmentEndPoint[3],const double segmentVector[3],double** intersections,int* intersectionsSize)
{
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    std::vector<double> _intersections;
    std::vector<double>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getTriangleSegmentCollision(_p,_v,_w,_segmentEndPoint,_segmentVector,_ints);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new double[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getMeshMeshDistanceIfSmaller(const void* mesh1ObbStruct,const double mesh1Transformation[7],const void* mesh2ObbStruct,const double mesh2Transformation[7],double* dist,double minDistSegPt1[3],double minDistSegPt2[3],int* mesh1Caching,int* mesh2Caching)
{
    C7Vector tr1;
    tr1.setData(mesh1Transformation);
    C7Vector tr2;
    tr2.setData(mesh2Transformation);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getMeshMeshDistanceIfSmaller((const CObbStruct*)mesh1ObbStruct,tr1,(const CObbStruct*)mesh2ObbStruct,tr2,dist[0],&_minDistSegPt1,&_minDistSegPt2,mesh1Caching,mesh2Caching);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const void* ocStruct,const double octreeTransformation[7],double* dist,double meshMinDistPt[3],double ocMinDistPt[3],int* meshCaching,unsigned long long int* ocCaching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C7Vector tr2;
    tr2.setData(octreeTransformation);
    C3Vector _meshMinDistPt;
    C3Vector _ocMinDistPt;
    bool retVal=geom_getMeshOctreeDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,(const COcStruct*)ocStruct,tr2,dist[0],&_meshMinDistPt,&_ocMinDistPt,meshCaching,ocCaching);
    if (retVal)
    {
        if (meshMinDistPt!=nullptr)
            _meshMinDistPt.getData(meshMinDistPt);
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getData(ocMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshPtcloudDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const void* pcStruct,const double pcTransformation[7],double* dist,double meshMinDistPt[3],double pcMinDistPt[3],int* meshCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C7Vector tr2;
    tr2.setData(pcTransformation);
    C3Vector _meshMinDistPt;
    C3Vector _pcMinDistPt;
    bool retVal=geom_getMeshPtcloudDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,(const CPcStruct*)pcStruct,tr2,dist[0],&_meshMinDistPt,&_pcMinDistPt,meshCaching,pcCaching);
    if (retVal)
    {
        if (meshMinDistPt!=nullptr)
            _meshMinDistPt.getData(meshMinDistPt);
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getData(pcMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const double p[3],const double v[3],const double w[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3],int* caching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getMeshTriangleDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,_p,_v,_w,dist[0],&_minDistSegPt1,&_minDistSegPt2,caching);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3],int* caching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getMeshSegmentDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,_segmentEndPoint,_segmentVector,dist[0],&_minDistSegPt1,&_minDistSegPt2,caching);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshPointDistanceIfSmaller(const void* meshObbStruct,const double meshTransformation[7],const double point[3],double* dist,double minDistSegPt[3],int* caching)
{
    C7Vector tr1;
    tr1.setData(meshTransformation);
    C3Vector _point(point);
    C3Vector _minDistSegPt;
    bool retVal=geom_getMeshPointDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,_point,dist[0],&_minDistSegPt,caching);
    if (retVal)
    {
        if (minDistSegPt!=nullptr)
            _minDistSegPt.getData(minDistSegPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const double octree1Transformation[7],const void* oc2Struct,const double octree2Transformation[7],double* dist,double oc1MinDistPt[3],double oc2MinDistPt[3],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching)
{
    C7Vector tr1;
    tr1.setData(octree1Transformation);
    C7Vector tr2;
    tr2.setData(octree2Transformation);
    C3Vector _oc1MinDistPt;
    C3Vector _oc2MinDistPt;
    bool retVal=geom_getOctreeOctreeDistanceIfSmaller((const COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,dist[0],&_oc1MinDistPt,&_oc2MinDistPt,oc1Caching,oc2Caching);
    if (retVal)
    {
        if (oc1MinDistPt!=nullptr)
            _oc1MinDistPt.getData(oc1MinDistPt);
        if (oc2MinDistPt!=nullptr)
            _oc2MinDistPt.getData(oc2MinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const void* pcStruct,const double pcTransformation[7],double* dist,double ocMinDistPt[3],double pcMinDistPt[3],unsigned long long int* ocCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setData(octreeTransformation);
    C7Vector tr2;
    tr2.setData(pcTransformation);
    C3Vector _ocMinDistPt;
    C3Vector _pcMinDistPt;
    bool retVal=geom_getOctreePtcloudDistanceIfSmaller((const COcStruct*)ocStruct,tr1,(const CPcStruct*)pcStruct,tr2,dist[0],&_ocMinDistPt,&_pcMinDistPt,ocCaching,pcCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getData(ocMinDistPt);
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getData(pcMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const double p[3],const double v[3],const double w[3],double* dist,double ocMinDistPt[3],double triMinDistPt[3],unsigned long long int* ocCaching)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _ocMinDistPt;
    C3Vector _triMinDistPt;
    bool retVal=geom_getOctreeTriangleDistanceIfSmaller((const COcStruct*)ocStruct,tr,_p,_v,_w,dist[0],&_ocMinDistPt,&_triMinDistPt,ocCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getData(ocMinDistPt);
        if (triMinDistPt!=nullptr)
            _triMinDistPt.getData(triMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double ocMinDistPt[3],double segMinDistPt[3],unsigned long long int* ocCaching)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _ocMinDistPt;
    C3Vector _segMinDistPt;
    bool retVal=geom_getOctreeSegmentDistanceIfSmaller((const COcStruct*)ocStruct,tr,_segmentEndPoint,_segmentVector,dist[0],&_ocMinDistPt,&_segMinDistPt,ocCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getData(ocMinDistPt);
        if (segMinDistPt!=nullptr)
            _segMinDistPt.getData(segMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const double octreeTransformation[7],const double point[3],double* dist,double ocMinDistPt[3],unsigned long long int* ocCaching)
{
    C7Vector tr;
    tr.setData(octreeTransformation);
    C3Vector _point(point);
    C3Vector _ocMinDistPt;
    bool retVal=geom_getOctreePointDistanceIfSmaller((const COcStruct*)ocStruct,tr,_point,dist[0],&_ocMinDistPt,ocCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getData(ocMinDistPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getPtcloudPtcloudDistanceIfSmaller(const void* pc1Struct,const double pc1Transformation[7],const void* pc2Struct,const double pc2Transformation[7],double* dist,double* pc1MinDistPt,double* pc2MinDistPt,unsigned long long int* pc1Caching,unsigned long long int* pc2Caching)
{
    C7Vector tr1;
    tr1.setData(pc1Transformation);
    C7Vector tr2;
    tr2.setData(pc2Transformation);
    C3Vector _pc1MinDistPt;
    C3Vector _pc2MinDistPt;
    bool retVal=geom_getPtcloudPtcloudDistanceIfSmaller((const CPcStruct*)pc1Struct,tr1,(const CPcStruct*)pc2Struct,tr2,dist[0],&_pc1MinDistPt,&_pc2MinDistPt,pc1Caching,pc2Caching);
    if (retVal)
    {
        if (pc1MinDistPt!=nullptr)
            _pc1MinDistPt.getData(pc1MinDistPt);
        if (pc2MinDistPt!=nullptr)
            _pc2MinDistPt.getData(pc2MinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getPtcloudTriangleDistanceIfSmaller(const void* pcStruct,const double pcTransformation[7],const double p[3],const double v[3],const double w[3],double* dist,double* pcMinDistPt,double* triMinDistPt,unsigned long long int* pcCaching)
{
    C7Vector tr;
    tr.setData(pcTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _pcMinDistPt;
    C3Vector _triMinDistPt;
    bool retVal=geom_getPtcloudTriangleDistanceIfSmaller((const CPcStruct*)pcStruct,tr,_p,_v,_w,dist[0],&_pcMinDistPt,&_triMinDistPt,pcCaching);
    if (retVal)
    {
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getData(pcMinDistPt);
        if (triMinDistPt!=nullptr)
            _triMinDistPt.getData(triMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getPtcloudSegmentDistanceIfSmaller(const void* pcStruct,const double pcTransformation[7],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double* pcMinDistPt,double* segMinDistPt,unsigned long long int* pcCaching)
{
    C7Vector tr;
    tr.setData(pcTransformation);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _pcMinDistPt;
    C3Vector _segMinDistPt;
    bool retVal=geom_getPtcloudSegmentDistanceIfSmaller((const CPcStruct*)pcStruct,tr,_segmentEndPoint,_segmentVector,dist[0],&_pcMinDistPt,&_segMinDistPt,pcCaching);
    if (retVal)
    {
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getData(pcMinDistPt);
        if (segMinDistPt!=nullptr)
            _segMinDistPt.getData(segMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getPtcloudPointDistanceIfSmaller(const void* pcStruct,const double pcTransformation[7],const double point[3],double* dist,double* pcMinDistPt,unsigned long long int* pcCaching)
{
    C7Vector tr;
    tr.setData(pcTransformation);
    C3Vector _point(point);
    C3Vector _pcMinDistPt;
    bool retVal=geom_getPtcloudPointDistanceIfSmaller((const CPcStruct*)pcStruct,tr,_point,dist[0],&_pcMinDistPt,pcCaching);
    if (retVal)
    {
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getData(pcMinDistPt);
    }
    return(retVal);
}

SIM_DLLEXPORT double geomPlugin_getApproxBoxBoxDistance(const double box1Transformation[7],const double box1HalfSize[3],const double box2Transformation[7],const double box2HalfSize[3])
{
    C7Vector tr1;
    tr1.setData(box1Transformation);
    C7Vector tr2;
    tr2.setData(box2Transformation);
    C3Vector _b1hs(box1HalfSize);
    C3Vector _b2hs(box2HalfSize);
    double retVal=geom_getApproxBoxBoxDistance(tr1,_b1hs,tr2,_b2hs);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getBoxBoxDistanceIfSmaller(const double box1Transformation[7],const double box1HalfSize[3],const double box2Transformation[7],const double box2HalfSize[3],bool boxesAreSolid,double* dist,double distSegPt1[3],double distSegPt2[3])
{
    C7Vector tr1;
    tr1.setData(box1Transformation);
    C7Vector tr2;
    tr2.setData(box2Transformation);
    C3Vector _b1hs(box1HalfSize);
    C3Vector _b2hs(box2HalfSize);
    C3Vector _distSegPt1;
    C3Vector _distSegPt2;
    bool retVal=geom_getBoxBoxDistanceIfSmaller(tr1,_b1hs,tr2,_b2hs,boxesAreSolid,dist[0],&_distSegPt1,&_distSegPt2);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getData(distSegPt1);
        if (distSegPt2!=nullptr)
            _distSegPt2.getData(distSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleDistanceIfSmaller(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double p[3],const double v[3],const double w[3],double* dist,double distSegPt1[3],double distSegPt2[3])
{
    C7Vector tr;
    tr.setData(boxTransformation);
    C3Vector _bhs(boxHalfSize);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _distSegPt1;
    C3Vector _distSegPt2;
    bool retVal=geom_getBoxTriangleDistanceIfSmaller(tr,_bhs,boxIsSolid,_p,_v,_w,dist[0],&_distSegPt1,&_distSegPt2);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getData(distSegPt1);
        if (distSegPt2!=nullptr)
            _distSegPt2.getData(distSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentDistanceIfSmaller(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double segmentEndPoint[3],const double segmentVector[3],double* dist,double distSegPt1[3],double distSegPt2[3])
{
    C7Vector tr;
    tr.setData(boxTransformation);
    C3Vector _bhs(boxHalfSize);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _distSegPt1;
    C3Vector _distSegPt2;
    bool retVal=geom_getBoxSegmentDistanceIfSmaller(tr,_bhs,boxIsSolid,_segmentEndPoint,_segmentVector,dist[0],&_distSegPt1,&_distSegPt2);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getData(distSegPt1);
        if (distSegPt2!=nullptr)
            _distSegPt2.getData(distSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxPointDistanceIfSmaller(const double boxTransformation[7],const double boxHalfSize[3],bool boxIsSolid,const double point[3],double* dist,double distSegPt1[3])
{
    C7Vector tr;
    tr.setData(boxTransformation);
    C3Vector _bhs(boxHalfSize);
    C3Vector _point(point);
    C3Vector _distSegPt1;
    bool retVal=geom_getBoxPointDistanceIfSmaller(tr,_bhs,boxIsSolid,_point,dist[0],&_distSegPt1);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getData(distSegPt1);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleDistanceIfSmaller(const double p1[3],const double v1[3],const double w1[3],const double p2[3],const double v2[3],const double w2[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3])
{
    C3Vector _p1(p1);
    C3Vector _v1(v1);
    C3Vector _w1(w1);
    C3Vector _p2(p2);
    C3Vector _v2(v2);
    C3Vector _w2(w2);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getTriangleTriangleDistanceIfSmaller(_p1,_v1,_w1,_p2,_v2,_w2,dist[0],&_minDistSegPt1,&_minDistSegPt2);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentDistanceIfSmaller(const double p[3],const double v[3],const double w[3],const double segmentEndPoint[3],const double segmentVector[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3])
{
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getTriangleSegmentDistanceIfSmaller(_p,_v,_w,_segmentEndPoint,_segmentVector,dist[0],&_minDistSegPt1,&_minDistSegPt2);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTrianglePointDistanceIfSmaller(const double p[3],const double v[3],const double w[3],const double point[3],double* dist,double minDistSegPt[3])
{
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _point(point);
    C3Vector _minDistSegPt;
    bool retVal=geom_getTrianglePointDistanceIfSmaller(_p,_v,_w,_point,dist[0],&_minDistSegPt);
    if (retVal)
    {
        if (minDistSegPt!=nullptr)
            _minDistSegPt.getData(minDistSegPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getSegmentSegmentDistanceIfSmaller(const double segment1EndPoint[3],const double segment1Vector[3],const double segment2EndPoint[3],const double segment2Vector[3],double* dist,double minDistSegPt1[3],double minDistSegPt2[3])
{
    C3Vector _segment1EndPoint(segment1EndPoint);
    C3Vector _segment1Vector(segment1Vector);
    C3Vector _segment2EndPoint(segment2EndPoint);
    C3Vector _segment2Vector(segment2Vector);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getSegmentSegmentDistanceIfSmaller(_segment1EndPoint,_segment1Vector,_segment2EndPoint,_segment2Vector,dist[0],&_minDistSegPt1,&_minDistSegPt2);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getSegmentPointDistanceIfSmaller(const double segmentEndPoint[3],const double segmentVector[3],const double point[3],double* dist,double minDistSegPt[3])
{
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _point(point);
    C3Vector _minDistSegPt;
    bool retVal=geom_getSegmentPointDistanceIfSmaller(_segmentEndPoint,_segmentVector,_point,dist[0],&_minDistSegPt);
    if (retVal)
    {
        if (minDistSegPt!=nullptr)
            _minDistSegPt.getData(minDistSegPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectMeshIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const void* obbStruct,const double meshTransformationRelative[7],double* dist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3])
{   // sensor is at the origin. meshTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setData(meshTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_volumeSensorDetectMeshIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,(const CObbStruct*)obbStruct,tr,dist[0],fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
        if (triN!=nullptr)
            _triN.getData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectOctreeIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const void* ocStruct,const double octreeTransformationRelative[7],double* dist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3])
{   // sensor is at the origin. octreeTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setData(octreeTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_volumeSensorDetectOctreeIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,(const COcStruct*)ocStruct,tr,dist[0],fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
        if (triN!=nullptr)
            _triN.getData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectPtcloudIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const void* pcStruct,const double ptcloudTransformationRelative[7],double* dist,bool fast,double detectPt[3])
{   // sensor is at the origin. ptcloudTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setData(ptcloudTransformationRelative);
    C3Vector _detectPt;
    bool retVal=geom_volumeSensorDetectPtcloudIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,(const CPcStruct*)pcStruct,tr,dist[0],fast,&_detectPt);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectTriangleIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const double p[3],const double v[3],const double w[3],double* dist,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3])
{   // sensor is at the origin. triangle is relative to the sensor
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_volumeSensorDetectTriangleIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,_p,_v,_w,dist[0],frontDetection,backDetection,maxAngle,&_detectPt,&_triN);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
        if (triN!=nullptr)
            _triN.getData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectSegmentIfSmaller(const double* planesIn,int planesInSize,const double* planesOut,int planesOutSize,const double segmentEndPoint[3],const double segmentVector[3],double* dist,double maxAngle,double detectPt[3])
{   // sensor is at the origin. segment is relative to the sensor
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _detectPt;
    bool retVal=geom_volumeSensorDetectSegmentIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,_segmentEndPoint,_segmentVector,dist[0],maxAngle,&_detectPt);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_raySensorDetectMeshIfSmaller(const double rayStart[3],const double rayVect[3],const void* obbStruct,const double meshTransformationRelative[7],double* dist,double forbiddenDist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3],bool* forbiddenDistTouched)
{   // sensor is at the origin. meshTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setData(meshTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_raySensorDetectMeshIfSmaller(C3Vector(rayStart),C3Vector(rayVect),(const CObbStruct*)obbStruct,tr,dist[0],forbiddenDist,fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN,forbiddenDistTouched);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
        if (triN!=nullptr)
            _triN.getData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_raySensorDetectOctreeIfSmaller(const double rayStart[3],const double rayVect[3],const void* ocStruct,const double octreeTransformationRelative[7],double* dist,double forbiddenDist,bool fast,bool frontDetection,bool backDetection,double maxAngle,double detectPt[3],double triN[3],bool* forbiddenDistTouched)
{   // sensor is at the origin. octreeTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setData(octreeTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_raySensorDetectOctreeIfSmaller(C3Vector(rayStart),C3Vector(rayVect),(const COcStruct*)ocStruct,tr,dist[0],forbiddenDist,fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN,forbiddenDistTouched);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getData(detectPt);
        if (triN!=nullptr)
            _triN.getData(triN);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_isPointInVolume(const double* planesIn,int planesInSize,const double point[3])
{
    bool retVal=geom_isPointInVolume(planesIn,planesInSize,C3Vector(point));
    return(retVal);
}
