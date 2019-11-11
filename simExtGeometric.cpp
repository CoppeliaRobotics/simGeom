#include "simExtGeometric.h"
#include "simLib.h"
#include "obbStruct.h"
#include "pcStruct.h"
#include "ocStruct.h"
#include <iostream>
#include <cstdio>
#include "calcUtils.h"
#include "geom.h"
#include "scriptFunctionData.h"
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

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

LIBRARY simLib;

std::map<int,CObbStruct*> _meshData;
int _nextMeshDataHandle=0;
std::map<int,COcStruct*> _octreeData;
int _nextOctreeDataHandle=0;
std::map<int,CPcStruct*> _ptcloudData;
int _nextPtcloudDataHandle=0;


// --------------------------------------------------------------------------------------
// simGeom.getMeshMeshCollision
// --------------------------------------------------------------------------------------
#define LUA_GETMESHMESHCOLLISION_COMMAND_PLUGIN "simGeom.getMeshMeshCollision@Geometric"
#define LUA_GETMESHMESHCOLLISION_COMMAND "simGeom.getMeshMeshCollision"

const int inArgs_GETMESHMESHCOLLISION[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32|sim_lua_arg_table,2, // cachData, default is nil
    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETMESHMESHCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHMESHCOLLISION,inArgs_GETMESHMESHCOLLISION[0]-2,LUA_GETMESHMESHCOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,CObbStruct*>::iterator it2=_meshData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_meshData.end()) )
        {
            C3Vector mesh1Pos(&(inData->at(1).floatData[0]));
            C4Vector mesh1Q(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector mesh2Pos(&(inData->at(4).floatData[0]));
            C4Vector mesh2Q(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
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
            std::vector<float>* _intersect=nullptr;
            std::vector<float> intersect;
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
            simSetLastError(LUA_GETMESHMESHCOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshOctreeCollision
// --------------------------------------------------------------------------------------
#define LUA_GETMESHOCTREECOLLISION_COMMAND_PLUGIN "simGeom.getMeshOctreeCollision@Geometric"
#define LUA_GETMESHOCTREECOLLISION_COMMAND "simGeom.getMeshOctreeCollision"

const int inArgs_GETMESHOCTREECOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHOCTREECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHOCTREECOLLISION,inArgs_GETMESHOCTREECOLLISION[0]-1,LUA_GETMESHOCTREECOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector octreePos(&(inData->at(4).floatData[0]));
            C4Vector octreeQ(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            int cache1=-1;
            unsigned long long int cache2=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()>=2) )
            {
                cache1=(int)inData->at(6).doubleData[0];
                cache2=(unsigned long long int)inData->at(6).doubleData[1];
            }
            bool coll=geom_getMeshOctreeCollision(it1->second,C7Vector(meshQ,meshPos),it2->second,C7Vector(octreeQ,octreePos),&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETMESHOCTREECOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// simGeom.getMeshTriangleCollision
// --------------------------------------------------------------------------------------
#define LUA_GETMESHTRIANGLECOLLISION_COMMAND_PLUGIN "simGeom.getMeshTriangleCollision@Geometric"
#define LUA_GETMESHTRIANGLECOLLISION_COMMAND "simGeom.getMeshTriangleCollision"

const int inArgs_GETMESHTRIANGLECOLLISION[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_int32,0, // cachData, default is -1
    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETMESHTRIANGLECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHTRIANGLECOLLISION,inArgs_GETMESHTRIANGLECOLLISION[0]-2,LUA_GETMESHTRIANGLECOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector triPt1(&inData->at(3).floatData[0]);
            C3Vector triPt2(&inData->at(4).floatData[0]);
            C3Vector triPt3(&inData->at(5).floatData[0]);
            int cache=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()==1) )
                cache=inData->at(6).int32Data[0];
            bool returnIntersections=false;
            if ( (inData->size()>=8)&&(inData->at(7).boolData.size()==1) )
                returnIntersections=inData->at(7).boolData[0];
            std::vector<float>* _intersect=nullptr;
            std::vector<float> intersect;
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
            simSetLastError(LUA_GETMESHTRIANGLECOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshSegmentCollision
// --------------------------------------------------------------------------------------
#define LUA_GETMESHSEGMENTCOLLISION_COMMAND_PLUGIN "simGeom.getMeshSegmentCollision@Geometric"
#define LUA_GETMESHSEGMENTCOLLISION_COMMAND "simGeom.getMeshSegmentCollision"

const int inArgs_GETMESHSEGMENTCOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_int32,0, // cachData, default is -1
    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETMESHSEGMENTCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHSEGMENTCOLLISION,inArgs_GETMESHSEGMENTCOLLISION[0]-2,LUA_GETMESHSEGMENTCOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector segPt1(&inData->at(3).floatData[0]);
            C3Vector segPt2(&inData->at(4).floatData[0]);
            int cache=-1;
            if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
                cache=inData->at(5).int32Data[0];
            bool returnIntersections=false;
            if ( (inData->size()>=7)&&(inData->at(6).boolData.size()==1) )
                returnIntersections=inData->at(6).boolData[0];
            std::vector<float>* _intersect=nullptr;
            std::vector<float> intersect;
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
            simSetLastError(LUA_GETMESHSEGMENTCOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeOctreeCollision
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREEOCTREECOLLISION_COMMAND_PLUGIN "simGeom.getOctreeOctreeCollision@Geometric"
#define LUA_GETOCTREEOCTREECOLLISION_COMMAND "simGeom.getOctreeOctreeCollision"

const int inArgs_GETOCTREEOCTREECOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
//    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETOCTREEOCTREECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEOCTREECOLLISION,inArgs_GETOCTREEOCTREECOLLISION[0]-1,LUA_GETOCTREEOCTREECOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector octree1Pos(&(inData->at(1).floatData[0]));
            C4Vector octree1Q(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector octree2Pos(&(inData->at(4).floatData[0]));
            C4Vector octree2Q(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            unsigned long long int cache1=-1;
            unsigned long long int cache2=-1;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()>=2) )
            {
                cache1=( unsigned long long int)inData->at(6).doubleData[0];
                cache2=( unsigned long long int)inData->at(6).doubleData[1];
            }
//            bool returnIntersections=false;
//            if ( (inData->size()>=8)&&(inData->at(7).boolData.size()==1) )
//                returnIntersections=inData->at(7).boolData[0];
//            std::vector<float>* _intersect=nullptr;
//            std::vector<float> intersect;
//            if (returnIntersections)
//                _intersect=&intersect;
            bool coll=geom_getOctreeOctreeCollision(it1->second,C7Vector(octree1Q,octree1Pos),it2->second,C7Vector(octree2Q,octree2Pos),&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
//                if (returnIntersections)
//                    D.pushOutData(CScriptFunctionDataItem(intersect));
            }
        }
        else
            simSetLastError(LUA_GETOCTREEOCTREECOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePtcloudCollision
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREEPTCLOUDCOLLISION_COMMAND_PLUGIN "simGeom.getOctreePtcloudCollision@Geometric"
#define LUA_GETOCTREEPTCLOUDCOLLISION_COMMAND "simGeom.getOctreePtcloudCollision"

const int inArgs_GETOCTREEPTCLOUDCOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
//    sim_script_arg_bool,0, // return intersections, default is false
};

void LUA_GETOCTREEPTCLOUDCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPTCLOUDCOLLISION,inArgs_GETOCTREEPTCLOUDCOLLISION[0]-1,LUA_GETOCTREEPTCLOUDCOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector ptcloudPos(&(inData->at(4).floatData[0]));
            C4Vector ptcloudQ(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            unsigned long long int cache1=-1;
            unsigned long long int cache2=-1;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()>=2) )
            {
                cache1=( unsigned long long int)inData->at(6).doubleData[0];
                cache2=( unsigned long long int)inData->at(6).doubleData[1];
            }
//            bool returnIntersections=false;
//            if ( (inData->size()>=8)&&(inData->at(7).boolData.size()==1) )
//                returnIntersections=inData->at(7).boolData[0];
//            std::vector<float>* _intersect=nullptr;
//            std::vector<float> intersect;
//            if (returnIntersections)
//                _intersect=&intersect;
            bool coll=geom_getOctreePtcloudCollision(it1->second,C7Vector(octreeQ,octreePos),it2->second,C7Vector(ptcloudQ,ptcloudPos),&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
            {
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
//                if (returnIntersections)
//                    D.pushOutData(CScriptFunctionDataItem(intersect));
            }
        }
        else
            simSetLastError(LUA_GETOCTREEPTCLOUDCOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeTriangleCollision
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREETRIANGLECOLLISION_COMMAND_PLUGIN "simGeom.getOctreeTriangleCollision@Geometric"
#define LUA_GETOCTREETRIANGLECOLLISION_COMMAND "simGeom.getOctreeTriangleCollision"

const int inArgs_GETOCTREETRIANGLECOLLISION[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREETRIANGLECOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREETRIANGLECOLLISION,inArgs_GETOCTREETRIANGLECOLLISION[0]-2,LUA_GETOCTREETRIANGLECOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector triPt1(&(inData->at(3).floatData[0]));
            C3Vector triPt2(&(inData->at(4).floatData[0]));
            C3Vector triPt3(&(inData->at(5).floatData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(6).doubleData[0];
            bool coll=geom_getOctreeTriangleCollision(it->second,C7Vector(octreeQ,octreePos),triPt1,triPt2-triPt1,triPt3-triPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem((double)cache));
        }
        else
            simSetLastError(LUA_GETOCTREETRIANGLECOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeSegmentCollision
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREESEGMENTCOLLISION_COMMAND_PLUGIN "simGeom.getOctreeSegmentCollision@Geometric"
#define LUA_GETOCTREESEGMENTCOLLISION_COMMAND "simGeom.getOctreeSegmentCollision"

const int inArgs_GETOCTREESEGMENTCOLLISION[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREESEGMENTCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREESEGMENTCOLLISION,inArgs_GETOCTREESEGMENTCOLLISION[0]-2,LUA_GETOCTREESEGMENTCOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector segPt1(&(inData->at(3).floatData[0]));
            C3Vector segPt2(&(inData->at(4).floatData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(5).doubleData[0];
            bool coll=geom_getOctreeSegmentCollision(it->second,C7Vector(octreeQ,octreePos),segPt1,segPt2-segPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem((double)cache));
        }
        else
            simSetLastError(LUA_GETOCTREESEGMENTCOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePointCollision
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREEPOINTCOLLISION_COMMAND_PLUGIN "simGeom.getOctreePointCollision@Geometric"
#define LUA_GETOCTREEPOINTCOLLISION_COMMAND "simGeom.getOctreePointCollision"

const int inArgs_GETOCTREEPOINTCOLLISION[]={
    5,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREEPOINTCOLLISION_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPOINTCOLLISION,inArgs_GETOCTREEPOINTCOLLISION[0]-2,LUA_GETOCTREEPOINTCOLLISION_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector point(&(inData->at(3).floatData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(4).doubleData[0];
            bool coll=geom_getOctreePointCollision(it->second,C7Vector(octreeQ,octreePos),point,nullptr,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem((double)cache));
        }
        else
            simSetLastError(LUA_GETOCTREEPOINTCOLLISION_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------
// simGeom.getMeshMeshDistance
// --------------------------------------------------------------------------------------
#define LUA_GETMESHMESHDISTANCE_COMMAND_PLUGIN "simGeom.getMeshMeshDistance@Geometric"
#define LUA_GETMESHMESHDISTANCE_COMMAND "simGeom.getMeshMeshDistance"

const int inArgs_GETMESHMESHDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_int32|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHMESHDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHMESHDISTANCE,inArgs_GETMESHMESHDISTANCE[0]-2,LUA_GETMESHMESHDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,CObbStruct*>::iterator it2=_meshData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_meshData.end()) )
        {
            C3Vector mesh1Pos(&(inData->at(1).floatData[0]));
            C4Vector mesh1Q(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector mesh2Pos(&(inData->at(4).floatData[0]));
            C4Vector mesh2Q(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
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
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<int> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETMESHMESHDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshOctreeDistance
// --------------------------------------------------------------------------------------
#define LUA_GETMESHOCTREEDISTANCE_COMMAND_PLUGIN "simGeom.getMeshOctreeDistance@Geometric"
#define LUA_GETMESHOCTREEDISTANCE_COMMAND "simGeom.getMeshOctreeDistance"

const int inArgs_GETMESHOCTREEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHOCTREEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHOCTREEDISTANCE,inArgs_GETMESHOCTREEDISTANCE[0]-2,LUA_GETMESHOCTREEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector octreePos(&(inData->at(4).floatData[0]));
            C4Vector octreeQ(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            int cache1=-1;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=(int)inData->at(7).doubleData[0];
                cache2=(unsigned long long int)inData->at(7).doubleData[1];
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshOctreeDistanceIfSmaller(it1->second,C7Vector(meshQ,meshPos),it2->second,C7Vector(octreeQ,octreePos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETMESHOCTREEDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshPtcloudDistance
// --------------------------------------------------------------------------------------
#define LUA_GETMESHPTCLOUDDISTANCE_COMMAND_PLUGIN "simGeom.getMeshPtcloudDistance@Geometric"
#define LUA_GETMESHPTCLOUDDISTANCE_COMMAND "simGeom.getMeshPtcloudDistance"

const int inArgs_GETMESHPTCLOUDDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETMESHPTCLOUDDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHPTCLOUDDISTANCE,inArgs_GETMESHPTCLOUDDISTANCE[0]-2,LUA_GETMESHPTCLOUDDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CObbStruct*>::iterator it1=_meshData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_meshData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector ptcloudPos(&(inData->at(4).floatData[0]));
            C4Vector ptcloudQ(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            int cache1=-1;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=(int)inData->at(7).doubleData[0];
                cache2=(unsigned long long int)inData->at(7).doubleData[1];
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshPtcloudDistanceIfSmaller(it1->second,C7Vector(meshQ,meshPos),it2->second,C7Vector(ptcloudQ,ptcloudPos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETMESHPTCLOUDDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeOctreeDistance
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREEOCTREEDISTANCE_COMMAND_PLUGIN "simGeom.getOctreeOctreeDistance@Geometric"
#define LUA_GETOCTREEOCTREEDISTANCE_COMMAND "simGeom.getOctreeOctreeDistance"

const int inArgs_GETOCTREEOCTREEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETOCTREEOCTREEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEOCTREEDISTANCE,inArgs_GETOCTREEOCTREEDISTANCE[0]-2,LUA_GETOCTREEOCTREEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,COcStruct*>::iterator it2=_octreeData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_octreeData.end()) )
        {
            C3Vector octree1Pos(&(inData->at(1).floatData[0]));
            C4Vector octree1Q(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector octree2Pos(&(inData->at(4).floatData[0]));
            C4Vector octree2Q(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=(unsigned long long int)inData->at(7).doubleData[0];
                cache2=(unsigned long long int)inData->at(7).doubleData[1];
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeOctreeDistanceIfSmaller(it1->second,C7Vector(octree1Q,octree1Pos),it2->second,C7Vector(octree2Q,octree2Pos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETOCTREEOCTREEDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePtcloudDistance
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND_PLUGIN "simGeom.getOctreePtcloudDistance@Geometric"
#define LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND "simGeom.getOctreePtcloudDistance"

const int inArgs_GETOCTREEPTCLOUDDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETOCTREEPTCLOUDDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPTCLOUDDISTANCE,inArgs_GETOCTREEPTCLOUDDISTANCE[0]-2,LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,COcStruct*>::iterator it1=_octreeData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_octreeData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector ptcloudPos(&(inData->at(4).floatData[0]));
            C4Vector ptcloudQ(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            unsigned long long int cache1=0;
            unsigned long long int cache2=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()>=2) )
            {
                cache1=(unsigned long long int)inData->at(7).doubleData[0];
                cache2=(unsigned long long int)inData->at(7).doubleData[1];
            }
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreePtcloudDistanceIfSmaller(it1->second,C7Vector(octreeQ,octreePos),it2->second,C7Vector(ptcloudQ,ptcloudPos),dist,&distPt1,&distPt2,&cache1,&cache2);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeTriangleDistance
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREETRIANGLEDISTANCE_COMMAND_PLUGIN "simGeom.getOctreeTriangleDistance@Geometric"
#define LUA_GETOCTREETRIANGLEDISTANCE_COMMAND "simGeom.getOctreeTriangleDistance"

const int inArgs_GETOCTREETRIANGLEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREETRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREETRIANGLEDISTANCE,inArgs_GETOCTREETRIANGLEDISTANCE[0]-2,LUA_GETOCTREETRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector triPt1(&(inData->at(3).floatData[0]));
            C3Vector triPt2(&(inData->at(4).floatData[0]));
            C3Vector triPt3(&(inData->at(5).floatData[0]));
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(7).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeTriangleDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(LUA_GETOCTREETRIANGLEDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeSegmentDistance
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREESEGMENTDISTANCE_COMMAND_PLUGIN "simGeom.getOctreeSegmentDistance@Geometric"
#define LUA_GETOCTREESEGMENTDISTANCE_COMMAND "simGeom.getOctreeSegmentDistance"

const int inArgs_GETOCTREESEGMENTDISTANCE[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREESEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREESEGMENTDISTANCE,inArgs_GETOCTREESEGMENTDISTANCE[0]-2,LUA_GETOCTREESEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector segPt1(&(inData->at(3).floatData[0]));
            C3Vector segPt2(&(inData->at(4).floatData[0]));
            float dist=FLT_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).floatData.size()==1) )
                dist=inData->at(5).floatData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(6).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeSegmentDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(LUA_GETOCTREESEGMENTDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreePointDistance
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREEPOINTDISTANCE_COMMAND_PLUGIN "simGeom.getOctreePointDistance@Geometric"
#define LUA_GETOCTREEPOINTDISTANCE_COMMAND "simGeom.getOctreePointDistance"

const int inArgs_GETOCTREEPOINTDISTANCE[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETOCTREEPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEPOINTDISTANCE,inArgs_GETOCTREEPOINTDISTANCE[0]-2,LUA_GETOCTREEPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector octreePos(&(inData->at(1).floatData[0]));
            C4Vector octreeQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector point(&(inData->at(3).floatData[0]));
            float dist=FLT_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).floatData.size()==1) )
                dist=inData->at(4).floatData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(5).doubleData[0];
            C3Vector distPt;
            bool smaller=geom_getOctreePointDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),point,dist,&distPt,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt.data,distPt.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(LUA_GETOCTREEPOINTDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshTriangleDistance
// --------------------------------------------------------------------------------------
#define LUA_GETMESHTRIANGLEDISTANCE_COMMAND_PLUGIN "simGeom.getMeshTriangleDistance@Geometric"
#define LUA_GETMESHTRIANGLEDISTANCE_COMMAND "simGeom.getMeshTriangleDistance"

const int inArgs_GETMESHTRIANGLEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_int32,0, // cachData, default is -1
};

void LUA_GETMESHTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHTRIANGLEDISTANCE,inArgs_GETMESHTRIANGLEDISTANCE[0]-2,LUA_GETMESHTRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector triPt1(&inData->at(3).floatData[0]);
            C3Vector triPt2(&inData->at(4).floatData[0]);
            C3Vector triPt3(&inData->at(5).floatData[0]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            int cache=-1;
            if ( (inData->size()>=8)&&(inData->at(7).int32Data.size()==1) )
                cache=inData->at(7).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshTriangleDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(cache));
            }
        }
        else
            simSetLastError(LUA_GETMESHTRIANGLEDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshSegmentDistance
// --------------------------------------------------------------------------------------
#define LUA_GETMESHSEGMENTDISTANCE_COMMAND_PLUGIN "simGeom.getMeshSegmentDistance@Geometric"
#define LUA_GETMESHSEGMENTDISTANCE_COMMAND "simGeom.getMeshSegmentDistance"

const int inArgs_GETMESHSEGMENTDISTANCE[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_int32,0, // cachData, default is -1
};

void LUA_GETMESHSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHSEGMENTDISTANCE,inArgs_GETMESHSEGMENTDISTANCE[0]-2,LUA_GETMESHSEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector segPt1(&inData->at(3).floatData[0]);
            C3Vector segPt2(&inData->at(4).floatData[0]);
            float dist=FLT_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).floatData.size()==1) )
                dist=inData->at(5).floatData[0];
            int cache=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()==1) )
                cache=inData->at(6).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshSegmentDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(cache));
            }
        }
        else
            simSetLastError(LUA_GETMESHSEGMENTDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshPointDistance
// --------------------------------------------------------------------------------------
#define LUA_GETMESHPOINTDISTANCE_COMMAND_PLUGIN "simGeom.getMeshPointDistance@Geometric"
#define LUA_GETMESHPOINTDISTANCE_COMMAND "simGeom.getMeshPointDistance"

const int inArgs_GETMESHPOINTDISTANCE[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_int32,0, // cachData, default is -1
};

void LUA_GETMESHPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHPOINTDISTANCE,inArgs_GETMESHPOINTDISTANCE[0]-2,LUA_GETMESHPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector pt1(&inData->at(3).floatData[0]);
            float dist=FLT_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).floatData.size()==1) )
                dist=inData->at(4).floatData[0];
            int cache=-1;
            if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
                cache=inData->at(5).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshPointDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),pt1,dist,&distPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem(cache));
            }
        }
        else
            simSetLastError(LUA_GETMESHPOINTDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxBoxDistance
// --------------------------------------------------------------------------------------
#define LUA_GETBOXBOXDISTANCE_COMMAND_PLUGIN "simGeom.getBoxBoxDistance@Geometric"
#define LUA_GETBOXBOXDISTANCE_COMMAND "simGeom.getBoxBoxDistance"

const int inArgs_GETBOXBOXDISTANCE[]={
    8,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_bool,0,
};

void LUA_GETBOXBOXDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXBOXDISTANCE,inArgs_GETBOXBOXDISTANCE[0]-1,LUA_GETBOXBOXDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector box1Pos(&(inData->at(0).floatData[0]));
        C4Vector box1Q(inData->at(1).floatData[3],inData->at(1).floatData[0],inData->at(1).floatData[1],inData->at(1).floatData[2]);
        C3Vector box1Hs(&(inData->at(2).floatData[0]));
        C3Vector box2Pos(&(inData->at(3).floatData[0]));
        C4Vector box2Q(inData->at(4).floatData[3],inData->at(4).floatData[0],inData->at(4).floatData[1],inData->at(4).floatData[2]);
        C3Vector box2Hs(&(inData->at(5).floatData[0]));
        bool boxesAreSolid=inData->at(6).boolData[0];
        bool altRoutine=false;
        if ( (inData->size()>7)&&(inData->at(7).boolData.size()==1) )
            altRoutine=inData->at(7).boolData[0];
        C3Vector distPt1,distPt2;
        float dist=geom_getBoxBoxDistance(C7Vector(box1Q,box1Pos),box1Hs,C7Vector(box2Q,box2Pos),box2Hs,boxesAreSolid,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> pp1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp1));
        std::vector<float> pp2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxTriangleDistance
// --------------------------------------------------------------------------------------
#define LUA_GETBOXTRIANGLEDISTANCE_COMMAND_PLUGIN "simGeom.getBoxTriangleDistance@Geometric"
#define LUA_GETBOXTRIANGLEDISTANCE_COMMAND "simGeom.getBoxTriangleDistance"

const int inArgs_GETBOXTRIANGLEDISTANCE[]={
    8,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
};

void LUA_GETBOXTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXTRIANGLEDISTANCE,inArgs_GETBOXTRIANGLEDISTANCE[0]-1,LUA_GETBOXTRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).floatData[0]));
        C4Vector boxQ(inData->at(1).floatData[3],inData->at(1).floatData[0],inData->at(1).floatData[1],inData->at(1).floatData[2]);
        C3Vector boxHs(&(inData->at(2).floatData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector p1(&(inData->at(4).floatData[0]));
        C3Vector p2(&(inData->at(5).floatData[0]));
        C3Vector p3(&(inData->at(6).floatData[0]));
        bool altRoutine=false;
        if ( (inData->size()>7)&&(inData->at(7).boolData.size()==1) )
            altRoutine=inData->at(7).boolData[0];
        C3Vector distPt1,distPt2;
        float dist=geom_getBoxTriangleDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,p1,p2-p1,p3-p1,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> pp1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp1));
        std::vector<float> pp2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxSegmentDistance
// --------------------------------------------------------------------------------------
#define LUA_GETBOXSEGMENTDISTANCE_COMMAND_PLUGIN "simGeom.getBoxSegmentDistance@Geometric"
#define LUA_GETBOXSEGMENTDISTANCE_COMMAND "simGeom.getBoxSegmentDistance"

const int inArgs_GETBOXSEGMENTDISTANCE[]={
    7,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
};

void LUA_GETBOXSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXSEGMENTDISTANCE,inArgs_GETBOXSEGMENTDISTANCE[0]-1,LUA_GETBOXSEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).floatData[0]));
        C4Vector boxQ(inData->at(1).floatData[3],inData->at(1).floatData[0],inData->at(1).floatData[1],inData->at(1).floatData[2]);
        C3Vector boxHs(&(inData->at(2).floatData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector segP1(&(inData->at(4).floatData[0]));
        C3Vector segP2(&(inData->at(5).floatData[0]));
        bool altRoutine=false;
        if ( (inData->size()>6)&&(inData->at(6).boolData.size()==1) )
            altRoutine=inData->at(6).boolData[0];
        C3Vector distPt1,distPt2;
        float dist=geom_getBoxSegmentDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,segP1,segP2-segP1,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<float> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getBoxPointDistance
// --------------------------------------------------------------------------------------
#define LUA_GETBOXPOINTDISTANCE_COMMAND_PLUGIN "simGeom.getBoxPointDistance@Geometric"
#define LUA_GETBOXPOINTDISTANCE_COMMAND "simGeom.getBoxPointDistance"

const int inArgs_GETBOXPOINTDISTANCE[]={
    5,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_float|sim_lua_arg_table,3,
};

void LUA_GETBOXPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXPOINTDISTANCE,inArgs_GETBOXPOINTDISTANCE[0],LUA_GETBOXPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).floatData[0]));
        C4Vector boxQ(inData->at(1).floatData[3],inData->at(1).floatData[0],inData->at(1).floatData[1],inData->at(1).floatData[2]);
        C3Vector boxHs(&(inData->at(2).floatData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector point(&(inData->at(4).floatData[0]));
        C3Vector distPt;
        float dist=geom_getBoxPointDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt.data,distPt.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTriangleTriangleDistance
// --------------------------------------------------------------------------------------
#define LUA_GETTRIANGLETRIANGLEDISTANCE_COMMAND_PLUGIN "simGeom.getTriangleTriangleDistance@Geometric"
#define LUA_GETTRIANGLETRIANGLEDISTANCE_COMMAND "simGeom.getTriangleTriangleDistance"

const int inArgs_GETTRIANGLETRIANGLEDISTANCE[]={
    6,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLETRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLETRIANGLEDISTANCE,inArgs_GETTRIANGLETRIANGLEDISTANCE[0],LUA_GETTRIANGLETRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tpa1(&(inData->at(0).floatData[0]));
        C3Vector tpa2(&(inData->at(1).floatData[0]));
        C3Vector tpa3(&(inData->at(2).floatData[0]));
        C3Vector tpb1(&(inData->at(3).floatData[0]));
        C3Vector tpb2(&(inData->at(4).floatData[0]));
        C3Vector tpb3(&(inData->at(5).floatData[0]));
        C3Vector distPt1,distPt2;
        float dist=geom_getTriangleTriangleDistance(tpa1,tpa2-tpa1,tpa3-tpa1,tpb1,tpb2-tpb1,tpb3-tpb1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<float> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTriangleSegmentDistance
// --------------------------------------------------------------------------------------
#define LUA_GETTRIANGLESEGMENTDISTANCE_COMMAND_PLUGIN "simGeom.getTriangleSegmentDistance@Geometric"
#define LUA_GETTRIANGLESEGMENTDISTANCE_COMMAND "simGeom.getTriangleSegmentDistance"

const int inArgs_GETTRIANGLESEGMENTDISTANCE[]={
    5,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLESEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLESEGMENTDISTANCE,inArgs_GETTRIANGLESEGMENTDISTANCE[0],LUA_GETTRIANGLESEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tp1(&(inData->at(0).floatData[0]));
        C3Vector tp2(&(inData->at(1).floatData[0]));
        C3Vector tp3(&(inData->at(2).floatData[0]));
        C3Vector segP1(&(inData->at(3).floatData[0]));
        C3Vector segP2(&(inData->at(4).floatData[0]));
        C3Vector distPt1,distPt2;
        float dist=geom_getTriangleSegmentDistance(tp1,tp2-tp1,tp3-tp1,segP1,segP2-segP1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<float> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTrianglePointDistance
// --------------------------------------------------------------------------------------
#define LUA_GETTRIANGLEPOINTDISTANCE_COMMAND_PLUGIN "simGeom.getTrianglePointDistance@Geometric"
#define LUA_GETTRIANGLEPOINTDISTANCE_COMMAND "simGeom.getTrianglePointDistance"

const int inArgs_GETTRIANGLEPOINTDISTANCE[]={
    4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLEPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLEPOINTDISTANCE,inArgs_GETTRIANGLEPOINTDISTANCE[0],LUA_GETTRIANGLEPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tp1(&(inData->at(0).floatData[0]));
        C3Vector tp2(&(inData->at(1).floatData[0]));
        C3Vector tp3(&(inData->at(2).floatData[0]));
        C3Vector point(&(inData->at(3).floatData[0]));
        C3Vector distPt;
        float dist=geom_getTrianglePointDistance(tp1,tp2-tp1,tp3-tp1,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt.data,distPt.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getSegmentSegmentDistance
// --------------------------------------------------------------------------------------
#define LUA_GETSEGMENTSEGMENTDISTANCE_COMMAND_PLUGIN "simGeom.getSegmentSegmentDistance@Geometric"
#define LUA_GETSEGMENTSEGMENTDISTANCE_COMMAND "simGeom.getSegmentSegmentDistance"

const int inArgs_GETSEGMENTSEGMENTDISTANCE[]={
    4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
};

void LUA_GETSEGMENTSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSEGMENTSEGMENTDISTANCE,inArgs_GETSEGMENTSEGMENTDISTANCE[0],LUA_GETSEGMENTSEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector segaP1(&(inData->at(0).floatData[0]));
        C3Vector segaP2(&(inData->at(1).floatData[0]));
        C3Vector segbP1(&(inData->at(2).floatData[0]));
        C3Vector segbP2(&(inData->at(3).floatData[0]));
        C3Vector distPt1,distPt2;
        float dist=geom_getSegmentSegmentDistance(segaP1,segaP2-segaP1,segbP1,segbP2-segbP1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<float> p2(distPt2.data,distPt2.data+3);
        D.pushOutData(CScriptFunctionDataItem(p2));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getSegmentPointDistance
// --------------------------------------------------------------------------------------
#define LUA_GETSEGMENTPOINTDISTANCE_COMMAND_PLUGIN "simGeom.getSegmentPointDistance@Geometric"
#define LUA_GETSEGMENTPOINTDISTANCE_COMMAND "simGeom.getSegmentPointDistance"

const int inArgs_GETSEGMENTPOINTDISTANCE[]={
    3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
};

void LUA_GETSEGMENTPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSEGMENTPOINTDISTANCE,inArgs_GETSEGMENTPOINTDISTANCE[0],LUA_GETSEGMENTPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector segPt1(&(inData->at(0).floatData[0]));
        C3Vector segPt2(&(inData->at(1).floatData[0]));
        C3Vector point(&(inData->at(3).floatData[0]));
        C3Vector distPt;
        float dist=geom_getSegmentPointDistance(segPt1,segPt2-segPt1,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<float> p1(distPt.data,distPt.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createMesh
// --------------------------------------------------------------------------------------
#define LUA_CREATEMESH_COMMAND_PLUGIN "simGeom.createMesh@Geometric"
#define LUA_CREATEMESH_COMMAND "simGeom.createMesh"

const int inArgs_CREATEMESH[]={
    6,
    sim_script_arg_float|sim_lua_arg_table,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0,
    sim_script_arg_int32,0,
};

void LUA_CREATEMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEMESH,inArgs_CREATEMESH[0]-4,LUA_CREATEMESH_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        float maxTriS=0.3f;
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=3)&&(inData->at(2).floatData.size()>=3) )
            pos.set(&inData->at(2).floatData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=4)&&(inData->at(3).floatData.size()>=4) )
        {
            quat(0)=inData->at(3).floatData[3];
            quat(1)=inData->at(3).floatData[0];
            quat(2)=inData->at(3).floatData[1];
            quat(3)=inData->at(3).floatData[2];
        }
        C7Vector tr(quat,pos);
        if ( (inData->size()>=5)&&(inData->at(4).floatData.size()==1) )
            maxTriS=inData->at(4).floatData[0];
        int maxTri=8;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
            maxTri=inData->at(5).int32Data[0];
        CObbStruct* obbStruct=geom_createMesh(&inData->at(0).floatData[0],inData->at(0).floatData.size(),&inData->at(1).int32Data[0],inData->at(1).int32Data.size(),&tr,maxTriS,maxTri);
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
#define LUA_DESTROYMESH_COMMAND_PLUGIN "simGeom.destroyMesh@Geometric"
#define LUA_DESTROYMESH_COMMAND "simGeom.destroyMesh"

const int inArgs_DESTROYMESH[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROYMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROYMESH,inArgs_DESTROYMESH[0],LUA_DESTROYMESH_COMMAND))
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
            simSetLastError(LUA_DESTROYMESH_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.copyMesh
// --------------------------------------------------------------------------------------
#define LUA_COPYMESH_COMMAND_PLUGIN "simGeom.copyMesh@Geometric"
#define LUA_COPYMESH_COMMAND "simGeom.copyMesh"

const int inArgs_COPYMESH[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_COPYMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYMESH,inArgs_COPYMESH[0],LUA_COPYMESH_COMMAND))
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
            simSetLastError(LUA_COPYMESH_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.scaleMesh
// --------------------------------------------------------------------------------------
#define LUA_SCALEMESH_COMMAND_PLUGIN "simGeom.scaleMesh@Geometric"
#define LUA_SCALEMESH_COMMAND "simGeom.scaleMesh"

const int inArgs_SCALEMESH[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
};

void LUA_SCALEMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SCALEMESH,inArgs_SCALEMESH[0],LUA_SCALEMESH_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
            geom_scaleMesh(it->second,inData->at(1).floatData[0]);
        else
            simSetLastError(LUA_SCALEMESH_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getMeshSerializationData
// --------------------------------------------------------------------------------------
#define LUA_GETMESHSERIALIZATIONDATA_COMMAND_PLUGIN "simGeom.getMeshSerializationData@Geometric"
#define LUA_GETMESHSERIALIZATIONDATA_COMMAND "simGeom.getMeshSerializationData"

const int inArgs_GETMESHSERIALIZATIONDATA[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETMESHSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETMESHSERIALIZATIONDATA,inArgs_GETMESHSERIALIZATIONDATA[0],LUA_GETMESHSERIALIZATIONDATA_COMMAND))
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
            simSetLastError(LUA_GETMESHSERIALIZATIONDATA_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createMeshFromSerializationData
// --------------------------------------------------------------------------------------
#define LUA_CREATEMESHFROMSERIALIZATIONDATA_COMMAND_PLUGIN "simGeom.createMeshFromSerializationData@Geometric"
#define LUA_CREATEMESHFROMSERIALIZATIONDATA_COMMAND "simGeom.createMeshFromSerializationData"

const int inArgs_CREATEMESHFROMSERIALIZATIONDATA[]={
    1,
    sim_script_arg_string,0,
};

void LUA_CREATEMESHFROMSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEMESHFROMSERIALIZATIONDATA,inArgs_CREATEMESHFROMSERIALIZATIONDATA[0],LUA_CREATEMESHFROMSERIALIZATIONDATA_COMMAND))
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
#define LUA_CREATEOCTREEFROMPOINTS_COMMAND_PLUGIN "simGeom.createOctreeFromPoints@Geometric"
#define LUA_CREATEOCTREEFROMPOINTS_COMMAND "simGeom.createOctreeFromPoints"

const int inArgs_CREATEOCTREEFROMPOINTS[]={
    6,
    sim_script_arg_float|sim_lua_arg_table,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0,
    sim_script_arg_int32|sim_lua_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_CREATEOCTREEFROMPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMPOINTS,inArgs_CREATEOCTREEFROMPOINTS[0]-5,LUA_CREATEOCTREEFROMPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).floatData.size()>=3) )
            pos.set(&inData->at(1).floatData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).floatData.size()>=4) )
        {
            quat(0)=inData->at(2).floatData[3];
            quat(1)=inData->at(2).floatData[0];
            quat(2)=inData->at(2).floatData[1];
            quat(3)=inData->at(2).floatData[2];
        }
        C7Vector tr(quat,pos);
        float cellS=0.05f;
        if ( (inData->size()>=4)&&(inData->at(3).floatData.size()==1) )
            cellS=inData->at(3).floatData[0];
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

        COcStruct* ocStruct=geom_createOctreeFromPoints(&inData->at(0).floatData[0],inData->at(0).floatData.size()/3,&tr,cellS,rgb,(unsigned int)usrdata);
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
#define LUA_CREATEOCTREEFROMCOLORPOINTS_COMMAND_PLUGIN "simGeom.createOctreeFromColorPoints@Geometric"
#define LUA_CREATEOCTREEFROMCOLORPOINTS_COMMAND "simGeom.createOctreeFromColorPoints"

const int inArgs_CREATEOCTREEFROMCOLORPOINTS[]={
    6,
    sim_script_arg_float|sim_lua_arg_table,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
};

void LUA_CREATEOCTREEFROMCOLORPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMCOLORPOINTS,inArgs_CREATEOCTREEFROMCOLORPOINTS[0]-5,LUA_CREATEOCTREEFROMCOLORPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).floatData.size()>=3) )
            pos.set(&inData->at(1).floatData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).floatData.size()>=4) )
        {
            quat(0)=inData->at(2).floatData[3];
            quat(1)=inData->at(2).floatData[0];
            quat(2)=inData->at(2).floatData[1];
            quat(3)=inData->at(2).floatData[2];
        }
        C7Vector tr(quat,pos);
        float cellS=0.05f;
        if ( (inData->size()>=4)&&(inData->at(3).floatData.size()==1) )
            cellS=inData->at(3).floatData[0];
        unsigned char* rgb=nullptr;
        std::vector<unsigned char> _rgb;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()>=inData->at(0).floatData.size()) )
        {
            _rgb.resize(inData->at(4).int32Data.size());
            for (size_t i=0;i<inData->at(4).int32Data.size();i++)
                _rgb[i]=(unsigned char)inData->at(4).int32Data[i];
            rgb=&_rgb[0];
        }
        unsigned int* usrdata=nullptr;
        std::vector<unsigned int> _usrdata;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=inData->at(0).floatData.size()/3) )
        {
            _usrdata.resize(inData->at(5).int32Data.size());
            for (size_t i=0;i<inData->at(5).int32Data.size();i++)
                _usrdata[i]=(unsigned int)inData->at(5).int32Data[i];
            usrdata=&_usrdata[0];
        }
        COcStruct* ocStruct=geom_createOctreeFromColorPoints(&inData->at(0).floatData[0],inData->at(0).floatData.size()/3,&tr,cellS,rgb,usrdata);
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
#define LUA_CREATEOCTREEFROMMESH_COMMAND_PLUGIN "simGeom.createOctreeFromMesh@Geometric"
#define LUA_CREATEOCTREEFROMMESH_COMMAND "simGeom.createOctreeFromMesh"

const int inArgs_CREATEOCTREEFROMMESH[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0,
    sim_script_arg_int32|sim_lua_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_CREATEOCTREEFROMMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMMESH,inArgs_CREATEOCTREEFROMMESH[0]-5,LUA_CREATEOCTREEFROMMESH_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CObbStruct*>::iterator it=_meshData.find(h);
        if (it!=_meshData.end())
        {
            C3Vector meshPos(&(inData->at(1).floatData[0]));
            C4Vector meshQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector pos;
            pos.clear();
            if ( (inData->size()>=4)&&(inData->at(3).floatData.size()>=3) )
                pos.set(&inData->at(3).floatData[0]);
            C4Vector quat;
            quat.setIdentity();
            if ( (inData->size()>=5)&&(inData->at(4).floatData.size()>=4) )
            {
                quat(0)=inData->at(4).floatData[3];
                quat(1)=inData->at(4).floatData[0];
                quat(2)=inData->at(4).floatData[1];
                quat(3)=inData->at(4).floatData[2];
            }
            C7Vector tr(quat,pos);
            float cellS=0.05f;
            if ( (inData->size()>=6)&&(inData->at(5).floatData.size()==1) )
                cellS=inData->at(5).floatData[0];
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
            simSetLastError(LUA_CREATEOCTREEFROMMESH_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromOctree
// --------------------------------------------------------------------------------------
#define LUA_CREATEOCTREEFROMOCTREE_COMMAND_PLUGIN "simGeom.createOctreeFromOctree@Geometric"
#define LUA_CREATEOCTREEFROMOCTREE_COMMAND "simGeom.createOctreeFromOctree"

const int inArgs_CREATEOCTREEFROMOCTREE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0,
    sim_script_arg_int32|sim_lua_arg_table,3,
    sim_script_arg_int32,0,
};

void LUA_CREATEOCTREEFROMOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMOCTREE,inArgs_CREATEOCTREEFROMOCTREE[0]-5,LUA_CREATEOCTREEFROMOCTREE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            C3Vector ocPos(&(inData->at(1).floatData[0]));
            C4Vector ocQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector pos;
            pos.clear();
            if ( (inData->size()>=4)&&(inData->at(3).floatData.size()>=3) )
                pos.set(&inData->at(3).floatData[0]);
            C4Vector quat;
            quat.setIdentity();
            if ( (inData->size()>=5)&&(inData->at(4).floatData.size()>=4) )
            {
                quat(0)=inData->at(4).floatData[3];
                quat(1)=inData->at(4).floatData[0];
                quat(2)=inData->at(4).floatData[1];
                quat(3)=inData->at(4).floatData[2];
            }
            C7Vector tr(quat,pos);
            float cellS=0.05f;
            if ( (inData->size()>=6)&&(inData->at(5).floatData.size()==1) )
                cellS=inData->at(5).floatData[0];
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
            simSetLastError(LUA_CREATEOCTREEFROMOCTREE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.destroyOctree
// --------------------------------------------------------------------------------------
#define LUA_DESTROYOCTREE_COMMAND_PLUGIN "simGeom.destroyOctree@Geometric"
#define LUA_DESTROYOCTREE_COMMAND "simGeom.destroyOctree"

const int inArgs_DESTROYOCTREE[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROYOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROYOCTREE,inArgs_DESTROYOCTREE[0],LUA_DESTROYOCTREE_COMMAND))
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
            simSetLastError(LUA_DESTROYOCTREE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.copyOctree
// --------------------------------------------------------------------------------------
#define LUA_COPYOCTREE_COMMAND_PLUGIN "simGeom.copyOctree@Geometric"
#define LUA_COPYOCTREE_COMMAND "simGeom.copyOctree"

const int inArgs_COPYOCTREE[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_COPYOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYOCTREE,inArgs_COPYOCTREE[0],LUA_COPYOCTREE_COMMAND))
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
            simSetLastError(LUA_COPYOCTREE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.scaleOctree
// --------------------------------------------------------------------------------------
#define LUA_SCALEOCTREE_COMMAND_PLUGIN "simGeom.scaleOctree@Geometric"
#define LUA_SCALEOCTREE_COMMAND "simGeom.scaleOctree"

const int inArgs_SCALEOCTREE[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
};

void LUA_SCALEOCTREE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SCALEOCTREE,inArgs_SCALEOCTREE[0],LUA_SCALEOCTREE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
            geom_scaleOctree(it->second,inData->at(1).floatData[0]);
        else
            simSetLastError(LUA_SCALEOCTREE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getOctreeSerializationData
// --------------------------------------------------------------------------------------
#define LUA_GETOCTREESERIALIZATIONDATA_COMMAND_PLUGIN "simGeom.getOctreeSerializationData@Geometric"
#define LUA_GETOCTREESERIALIZATIONDATA_COMMAND "simGeom.getOctreeSerializationData"

const int inArgs_GETOCTREESERIALIZATIONDATA[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETOCTREESERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREESERIALIZATIONDATA,inArgs_GETOCTREESERIALIZATIONDATA[0],LUA_GETOCTREESERIALIZATIONDATA_COMMAND))
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
            simSetLastError(LUA_GETOCTREESERIALIZATIONDATA_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createOctreeFromSerializationData
// --------------------------------------------------------------------------------------
#define LUA_CREATEOCTREEFROMSERIALIZATIONDATA_COMMAND_PLUGIN "simGeom.createOctreeFromSerializationData@Geometric"
#define LUA_CREATEOCTREEFROMSERIALIZATIONDATA_COMMAND "simGeom.createOctreeFromSerializationData"

const int inArgs_CREATEOCTREEFROMSERIALIZATIONDATA[]={
    1,
    sim_script_arg_string,0,
};

void LUA_CREATEOCTREEFROMSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEOCTREEFROMSERIALIZATIONDATA,inArgs_CREATEOCTREEFROMSERIALIZATIONDATA[0],LUA_CREATEOCTREEFROMSERIALIZATIONDATA_COMMAND))
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
#define LUA_GETOCTREEVOXELS_COMMAND_PLUGIN "simGeom.getOctreeVoxels@Geometric"
#define LUA_GETOCTREEVOXELS_COMMAND "simGeom.getOctreeVoxels"

const int inArgs_GETOCTREEVOXELS[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETOCTREEVOXELS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETOCTREEVOXELS,inArgs_GETOCTREEVOXELS[0],LUA_GETOCTREEVOXELS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,COcStruct*>::iterator it=_octreeData.find(h);
        if (it!=_octreeData.end())
        {
            std::vector<float> data;
            std::vector<unsigned int> usrData;
            geom_getOctreeVoxelData(it->second,data,&usrData);
            std::vector<float> pos;
            std::vector<int> rgb;
            for (size_t i=0;i<data.size()/6;i++)
            {
                pos.push_back(data[6*i+0]);
                pos.push_back(data[6*i+1]);
                pos.push_back(data[6*i+2]);
                rgb.push_back(int(data[6*i+3]*255.1f));
                rgb.push_back(int(data[6*i+4]*255.1f));
                rgb.push_back(int(data[6*i+5]*255.1f));
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
            simSetLastError(LUA_GETOCTREEVOXELS_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getTransformedPoints
// --------------------------------------------------------------------------------------
#define LUA_GETTRANSFORMEDPOINTS_COMMAND_PLUGIN "simGeom.getTransformedPoints@Geometric"
#define LUA_GETTRANSFORMEDPOINTS_COMMAND "simGeom.getTransformedPoints"

const int inArgs_GETTRANSFORMEDPOINTS[]={
    3,
    sim_script_arg_float|sim_lua_arg_table,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
};

void LUA_GETTRANSFORMEDPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRANSFORMEDPOINTS,inArgs_GETTRANSFORMEDPOINTS[0],LUA_GETTRANSFORMEDPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::vector<float> outData;
        outData.resize(inData->at(0).floatData.size());
        C3Vector pos(&(inData->at(1).floatData[0]));
        C4Vector q(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
        C7Vector tr(q,pos);
        for (size_t i=0;i<inData->at(0).floatData.size()/3;i++)
        {
            C3Vector v(&inData->at(0).floatData[0]+3*i);
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
#define LUA_CREATEPTCLOUDFROMPOINTS_COMMAND_PLUGIN "simGeom.createPtcloudFromPoints@Geometric"
#define LUA_CREATEPTCLOUDFROMPOINTS_COMMAND "simGeom.createPtcloudFromPoints"

const int inArgs_CREATEPTCLOUDFROMPOINTS[]={
    7,
    sim_script_arg_float|sim_lua_arg_table,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // cell size
    sim_script_arg_int32,0, // max points in cell
    sim_script_arg_int32|sim_lua_arg_table,3, // color
    sim_script_arg_float,0, // proximity tolerance
};

void LUA_CREATEPTCLOUDFROMPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMPOINTS,inArgs_CREATEPTCLOUDFROMPOINTS[0]-6,LUA_CREATEPTCLOUDFROMPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).floatData.size()>=3) )
            pos.set(&inData->at(1).floatData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).floatData.size()>=4) )
        {
            quat(0)=inData->at(2).floatData[3];
            quat(1)=inData->at(2).floatData[0];
            quat(2)=inData->at(2).floatData[1];
            quat(3)=inData->at(2).floatData[2];
        }
        C7Vector tr(quat,pos);
        float cellS=0.05f;
        if ( (inData->size()>=4)&&(inData->at(3).floatData.size()==1) )
            cellS=inData->at(3).floatData[0];
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
        float proxTol=0.005f;
        if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
            proxTol=inData->at(6).floatData[0];
        CPcStruct* pcStruct=geom_createPtcloudFromPoints(&inData->at(0).floatData[0],inData->at(0).floatData.size()/3,&tr,cellS,maxPts,rgb,proxTol);
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
#define LUA_CREATEPTCLOUDFROMCOLORPOINTS_COMMAND_PLUGIN "simGeom.createPtcloudFromColorPoints@Geometric"
#define LUA_CREATEPTCLOUDFROMCOLORPOINTS_COMMAND "simGeom.createPtcloudFromColorPoints"

const int inArgs_CREATEPTCLOUDFROMCOLORPOINTS[]={
    7,
    sim_script_arg_float|sim_lua_arg_table,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // cell size
    sim_script_arg_int32,0, // max points in cell
    sim_script_arg_int32|sim_lua_arg_table,0, // color
    sim_script_arg_float,0, // proximity tolerance
};

void LUA_CREATEPTCLOUDFROMCOLORPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMCOLORPOINTS,inArgs_CREATEPTCLOUDFROMCOLORPOINTS[0]-5,LUA_CREATEPTCLOUDFROMCOLORPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).floatData.size()>=3) )
            pos.set(&inData->at(1).floatData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).floatData.size()>=4) )
        {
            quat(0)=inData->at(2).floatData[3];
            quat(1)=inData->at(2).floatData[0];
            quat(2)=inData->at(2).floatData[1];
            quat(3)=inData->at(2).floatData[2];
        }
        C7Vector tr(quat,pos);
        float cellS=0.05f;
        if ( (inData->size()>=4)&&(inData->at(3).floatData.size()==1) )
            cellS=inData->at(3).floatData[0];
        int maxPts=20;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()==1) )
            maxPts=inData->at(4).int32Data[0];
        unsigned char* rgb=nullptr;
        std::vector<unsigned char> _rgb;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=inData->at(0).floatData.size()) )
        {
            _rgb.resize(inData->at(5).int32Data.size());
            for (size_t i=0;i<inData->at(5).int32Data.size();i++)
                _rgb[i]=(unsigned char)inData->at(5).int32Data[i];
            rgb=&_rgb[0];
        }
        float proxTol=0.005f;
        if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
            proxTol=inData->at(6).floatData[0];
        CPcStruct* pcStruct=geom_createPtcloudFromColorPoints(&inData->at(0).floatData[0],inData->at(0).floatData.size()/3,&tr,cellS,maxPts,rgb,proxTol);
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
#define LUA_DESTROYPTCLOUD_COMMAND_PLUGIN "simGeom.destroyPtcloud@Geometric"
#define LUA_DESTROYPTCLOUD_COMMAND "simGeom.destroyPtcloud"

const int inArgs_DESTROYPTCLOUD[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_DESTROYPTCLOUD_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_DESTROYPTCLOUD,inArgs_DESTROYPTCLOUD[0],LUA_DESTROYPTCLOUD_COMMAND))
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
            simSetLastError(LUA_DESTROYPTCLOUD_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.copyPtcloud
// --------------------------------------------------------------------------------------
#define LUA_COPYPTCLOUD_COMMAND_PLUGIN "simGeom.copyPtcloud@Geometric"
#define LUA_COPYPTCLOUD_COMMAND "simGeom.copyPtcloud"

const int inArgs_COPYPTCLOUD[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_COPYPTCLOUD_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_COPYPTCLOUD,inArgs_COPYPTCLOUD[0],LUA_COPYPTCLOUD_COMMAND))
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
            simSetLastError(LUA_COPYPTCLOUD_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.scalePtcloud
// --------------------------------------------------------------------------------------
#define LUA_SCALEPTCLOUD_COMMAND_PLUGIN "simGeom.scalePtcloud@Geometric"
#define LUA_SCALEPTCLOUD_COMMAND "simGeom.scalePtcloud"

const int inArgs_SCALEPTCLOUD[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
};

void LUA_SCALEPTCLOUD_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_SCALEPTCLOUD,inArgs_SCALEPTCLOUD[0],LUA_SCALEPTCLOUD_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
            geom_scalePtcloud(it->second,inData->at(1).floatData[0]);
        else
            simSetLastError(LUA_SCALEPTCLOUD_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudSerializationData
// --------------------------------------------------------------------------------------
#define LUA_GETPTCLOUDSERIALIZATIONDATA_COMMAND_PLUGIN "simGeom.getPtcloudSerializationData@Geometric"
#define LUA_GETPTCLOUDSERIALIZATIONDATA_COMMAND "simGeom.getPtcloudSerializationData"

const int inArgs_GETPTCLOUDSERIALIZATIONDATA[]={
    1,
    sim_script_arg_int32,0,
};

void LUA_GETPTCLOUDSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDSERIALIZATIONDATA,inArgs_GETPTCLOUDSERIALIZATIONDATA[0],LUA_GETPTCLOUDSERIALIZATIONDATA_COMMAND))
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
            simSetLastError(LUA_GETPTCLOUDSERIALIZATIONDATA_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.createPtcloudFromSerializationData
// --------------------------------------------------------------------------------------
#define LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_COMMAND_PLUGIN "simGeom.createPtcloudFromSerializationData@Geometric"
#define LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_COMMAND "simGeom.createPtcloudFromSerializationData"

const int inArgs_CREATEPTCLOUDFROMSERIALIZATIONDATA[]={
    1,
    sim_script_arg_string,0,
};

void LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMSERIALIZATIONDATA,inArgs_CREATEPTCLOUDFROMSERIALIZATIONDATA[0],LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_COMMAND))
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
#define LUA_GETPTCLOUDPOINTS_COMMAND_PLUGIN "simGeom.getPtcloudPoints@Geometric"
#define LUA_GETPTCLOUDPOINTS_COMMAND "simGeom.getPtcloudPoints"

const int inArgs_GETPTCLOUDPOINTS[]={
    2,
    sim_script_arg_int32,0,
    sim_script_arg_float,0,
};

void LUA_GETPTCLOUDPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDPOINTS,inArgs_GETPTCLOUDPOINTS[0]-1,LUA_GETPTCLOUDPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            float prop=1.0f;
            if ( (inData->size()>=2)&&(inData->at(1).floatData.size()==1) )
                prop=inData->at(1).floatData[0];
            std::vector<float> data;
            geom_getPtcloudPoints(it->second,data,prop);
            std::vector<float> pos;
            std::vector<int> rgb;
            for (size_t i=0;i<data.size()/6;i++)
            {
                pos.push_back(data[6*i+0]);
                pos.push_back(data[6*i+1]);
                pos.push_back(data[6*i+2]);
                rgb.push_back(int(data[6*i+3]*255.1f));
                rgb.push_back(int(data[6*i+4]*255.1f));
                rgb.push_back(int(data[6*i+5]*255.1f));
            }
            D.pushOutData(CScriptFunctionDataItem(pos));
            D.pushOutData(CScriptFunctionDataItem(rgb));
        }
        else
            simSetLastError(LUA_GETPTCLOUDPOINTS_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudPtcloudDistance
// --------------------------------------------------------------------------------------
#define LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND_PLUGIN "simGeom.getPtcloudPtcloudDistance@Geometric"
#define LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND "simGeom.getPtcloudPtcloudDistance"

const int inArgs_GETPTCLOUDPTCLOUDDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double|sim_lua_arg_table,2, // cachData, default is nil
};

void LUA_GETPTCLOUDPTCLOUDDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDPTCLOUDDISTANCE,inArgs_GETPTCLOUDPTCLOUDDISTANCE[0]-2,LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h1=inData->at(0).int32Data[0];
        int h2=inData->at(3).int32Data[0];
        std::map<int,CPcStruct*>::iterator it1=_ptcloudData.find(h1);
        std::map<int,CPcStruct*>::iterator it2=_ptcloudData.find(h2);
        if ( (it1!=_ptcloudData.end())&&(it2!=_ptcloudData.end()) )
        {
            C3Vector ptcloud1Pos(&(inData->at(1).floatData[0]));
            C4Vector ptcloud1Q(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector ptcloud2Pos(&(inData->at(4).floatData[0]));
            C4Vector ptcloud2Q(inData->at(5).floatData[3],inData->at(5).floatData[0],inData->at(5).floatData[1],inData->at(5).floatData[2]);
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
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
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(cache1);
                c.push_back(cache2);
                D.pushOutData(CScriptFunctionDataItem(c));
            }
        }
        else
            simSetLastError(LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudTriangleDistance
// --------------------------------------------------------------------------------------
#define LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND_PLUGIN "simGeom.getPtcloudTriangleDistance@Geometric"
#define LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND "simGeom.getPtcloudTriangleDistance"

const int inArgs_GETPTCLOUDTRIANGLEDISTANCE[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETPTCLOUDTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDTRIANGLEDISTANCE,inArgs_GETPTCLOUDTRIANGLEDISTANCE[0]-2,LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            C3Vector ptcloudPos(&(inData->at(1).floatData[0]));
            C4Vector ptcloudQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector triPt1(&(inData->at(3).floatData[0]));
            C3Vector triPt2(&(inData->at(4).floatData[0]));
            C3Vector triPt3(&(inData->at(5).floatData[0]));
            float dist=FLT_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).floatData.size()==1) )
                dist=inData->at(6).floatData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(7).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudTriangleDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudSegmentDistance
// --------------------------------------------------------------------------------------
#define LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND_PLUGIN "simGeom.getPtcloudSegmentDistance@Geometric"
#define LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND "simGeom.getPtcloudSegmentDistance"

const int inArgs_GETPTCLOUDSEGMENTDISTANCE[]={
    7,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETPTCLOUDSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDSEGMENTDISTANCE,inArgs_GETPTCLOUDSEGMENTDISTANCE[0]-2,LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            C3Vector ptcloudPos(&(inData->at(1).floatData[0]));
            C4Vector ptcloudQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector segPt1(&(inData->at(3).floatData[0]));
            C3Vector segPt2(&(inData->at(4).floatData[0]));
            float dist=FLT_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).floatData.size()==1) )
                dist=inData->at(5).floatData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(6).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudSegmentDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<float> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// simGeom.getPtcloudPointDistance
// --------------------------------------------------------------------------------------
#define LUA_GETPTCLOUDPOINTDISTANCE_COMMAND_PLUGIN "simGeom.getPtcloudPointDistance@Geometric"
#define LUA_GETPTCLOUDPOINTDISTANCE_COMMAND "simGeom.getPtcloudPointDistance"

const int inArgs_GETPTCLOUDPOINTDISTANCE[]={
    6,
    sim_script_arg_int32,0,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float|sim_lua_arg_table,4,
    sim_script_arg_float|sim_lua_arg_table,3,
    sim_script_arg_float,0, // threshold, default is FLT_MAX
    sim_script_arg_double,0, // cachData, default is nil
};

void LUA_GETPTCLOUDPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETPTCLOUDPOINTDISTANCE,inArgs_GETPTCLOUDPOINTDISTANCE[0]-2,LUA_GETPTCLOUDPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        int h=inData->at(0).int32Data[0];
        std::map<int,CPcStruct*>::iterator it=_ptcloudData.find(h);
        if (it!=_ptcloudData.end())
        {
            C3Vector ptcloudPos(&(inData->at(1).floatData[0]));
            C4Vector ptcloudQ(inData->at(2).floatData[3],inData->at(2).floatData[0],inData->at(2).floatData[1],inData->at(2).floatData[2]);
            C3Vector point(&(inData->at(3).floatData[0]));
            float dist=FLT_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).floatData.size()==1) )
                dist=inData->at(4).floatData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(5).doubleData[0];
            C3Vector distPt;
            bool smaller=geom_getPtcloudPointDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),point,dist,&distPt,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<float> pp1(distPt.data,distPt.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem((double)cache));
            }
        }
        else
            simSetLastError(LUA_GETPTCLOUDPOINTDISTANCE_COMMAND,"Invalid handle.");
    }
    D.writeDataToStack(p->stackID);
}
// --------------------------------------------------------------------------------------

SIM_DLLEXPORT unsigned char simStart(void*,int)
{
    char curDirAndFile[1024];
#ifdef _WIN32
    #ifdef QT_COMPIL
        _getcwd(curDirAndFile, sizeof(curDirAndFile));
    #else
        GetModuleFileName(NULL,curDirAndFile,1023);
        PathRemoveFileSpec(curDirAndFile);
    #endif
#elif defined (__linux) || defined (__APPLE__)
    getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif

    std::string currentDirAndPath(curDirAndFile);
    std::string temp(currentDirAndPath);

#ifdef _WIN32
    temp+="\\coppeliaSim.dll";
#elif defined (__linux)
    temp+="/libcoppeliaSim.so";
#elif defined (__APPLE__)
    temp+="/libcoppeliaSim.dylib";
#endif /* __linux || __APPLE__ */

    simLib=loadSimLibrary(temp.c_str());
    if (simLib==NULL)
    {
        std::cout << "Error, could not find or correctly load the CoppeliaSim library. Cannot start 'Geometric' plugin.\n";
        return(0);
    }
    if (getSimProcAddresses(simLib)==0)
    {
        std::cout << "Error, could not find all required functions in the CoppeliaSim library. Cannot start 'Geometric' plugin.\n";
        unloadSimLibrary(simLib);
        return(0);
    }

    simRegisterScriptVariable("simGeom","require('simExtGeometric')",0);

    // Register the new Lua commands:
    simRegisterScriptCallbackFunction(LUA_GETMESHMESHCOLLISION_COMMAND_PLUGIN,strConCat("bool collision,table_2 cache,table intersections=",LUA_GETMESHMESHCOLLISION_COMMAND,"(\nnumber mesh1Handle,table_3 mesh1Pos,table_4 mesh1Quaternion,\nnumber mesh2Handle,table_3 mesh2Pos,table_4 mesh2Quaternion,\ntable_2 cache=nil,bool returnIntersections=false)"),LUA_GETMESHMESHCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHOCTREECOLLISION_COMMAND_PLUGIN,strConCat("bool collision,table_2 cache=",LUA_GETMESHOCTREECOLLISION_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_2 cache=nil)"),LUA_GETMESHOCTREECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHTRIANGLECOLLISION_COMMAND_PLUGIN,strConCat("bool collision,number cache,table intersections=",LUA_GETMESHTRIANGLECOLLISION_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber cache=-1,bool returnIntersections=false)"),LUA_GETMESHTRIANGLECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHSEGMENTCOLLISION_COMMAND_PLUGIN,strConCat("bool collision,number cache,table intersections=",LUA_GETMESHSEGMENTCOLLISION_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 segmentPt1,table_3 segmentPt2,\nnumber cache=-1,bool returnIntersections=false)"),LUA_GETMESHSEGMENTCOLLISION_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETOCTREEOCTREECOLLISION_COMMAND_PLUGIN,strConCat("bool collision,table_2 cache=",LUA_GETOCTREEOCTREECOLLISION_COMMAND,"(\nnumber octree1Handle,table_3 octree1Pos,table_4 octree1Quaternion,\nnumber octree2Handle,table_3 octree2Pos,table_4 octree2Quaternion,\ntable_2 cache=nil)"),LUA_GETOCTREEOCTREECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEPTCLOUDCOLLISION_COMMAND_PLUGIN,strConCat("bool collision,table_2 cache=",LUA_GETOCTREEPTCLOUDCOLLISION_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_2 cache=nil)"),LUA_GETOCTREEPTCLOUDCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREETRIANGLECOLLISION_COMMAND_PLUGIN,strConCat("bool collision,number cache=",LUA_GETOCTREETRIANGLECOLLISION_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber cache=-1)"),LUA_GETOCTREETRIANGLECOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREESEGMENTCOLLISION_COMMAND_PLUGIN,strConCat("bool collision,number cache=",LUA_GETOCTREESEGMENTCOLLISION_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 segPt1,table_3 segPt2,\nnumber cache=-1)"),LUA_GETOCTREESEGMENTCOLLISION_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEPOINTCOLLISION_COMMAND_PLUGIN,strConCat("bool collision,number cache=",LUA_GETOCTREEPOINTCOLLISION_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 point,number cache=-1)"),LUA_GETOCTREEPOINTCOLLISION_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETMESHMESHDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETMESHMESHDISTANCE_COMMAND,"(\nnumber mesh1Handle,table_3 mesh1Pos,table_4 mesh1Quaternion,\nnumber mesh2Handle,table_3 mesh2Pos,table_4 mesh2Quaternion,\nnumber distanceThreshold=FLT_MAX,table_2 cache=nil)"),LUA_GETMESHMESHDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHOCTREEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETMESHOCTREEDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\nnumber distanceThreshold=FLT_MAX,table_2 cache=nil)"),LUA_GETMESHOCTREEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHPTCLOUDDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETMESHPTCLOUDDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\nnumber distanceThreshold=FLT_MAX,table_2 cache=nil)"),LUA_GETMESHPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHTRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETMESHTRIANGLEDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETMESHTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHSEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETMESHSEGMENTDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 segmentPt1,table_3 segmentPt2,\nnumber distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETMESHSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt,number cache=",LUA_GETMESHPOINTDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 point,number distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETMESHPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETOCTREEOCTREEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETOCTREEOCTREEDISTANCE_COMMAND,"(\nnumber octree1Handle,table_3 octree1Pos,table_4 octree1Quaternion,\nnumber octree2Handle,table_3 octree2Pos,table_4 octree2Quaternion,\nnumber distanceThreshold=FLT_MAX,table_2 cache=nil)"),LUA_GETOCTREEOCTREEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\nnumber distanceThreshold=FLT_MAX,table_2 cache=nil)"),LUA_GETOCTREEPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREETRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETOCTREETRIANGLEDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETOCTREETRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREESEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETOCTREESEGMENTDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 segPt1,table_3 segPt2,\nnumber distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETOCTREESEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt,number cache=",LUA_GETOCTREEPOINTDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 point,number distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETOCTREEPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND,"(\nnumber ptcloud1Handle,table_3 ptcloud1Pos,table_4 ptcloud1Quaternion,\nnumber ptcloud2Handle,table_3 ptcloud2Pos,table_4 ptcloud2Quaternion,\nnumber distanceThreshold=FLT_MAX,table_2 cache=nil)"),LUA_GETPTCLOUDPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND,"(\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETPTCLOUDTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND,"(\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_3 segPt1,table_3 segPt2,\nnumber distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETPTCLOUDSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt,number cache=",LUA_GETPTCLOUDPOINTDISTANCE_COMMAND,"(\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_3 point,number distanceThreshold=FLT_MAX,number cache=-1)"),LUA_GETPTCLOUDPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETBOXBOXDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2=",LUA_GETBOXBOXDISTANCE_COMMAND,"(\ntable_3 box1Pos,table_4 box1Quaternion,table_3 box1HalfSize,table_3 box2Pos,\ntable_4 box2Quaternion,table_3 box2HalfSize,bool boxesAreSolid)"),LUA_GETBOXBOXDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETBOXTRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2=",LUA_GETBOXTRIANGLEDISTANCE_COMMAND,"(\ntable_3 boxPos,table_4 boxQuaternion,table_3 boxHalfSize,bool boxIsSolid,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,bool altRoutine=false)"),LUA_GETBOXTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETBOXSEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2=",LUA_GETBOXSEGMENTDISTANCE_COMMAND,"(\ntable_3 boxPos,table_4 boxQuaternion,table_3 boxHalfSize,bool boxIsSolid\n,table_3 segmentPt1,table_3 segmentPt2,bool altRoutine=false)"),LUA_GETBOXSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETBOXPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt=",LUA_GETBOXPOINTDISTANCE_COMMAND,"(\ntable_3 boxPos,table_4 boxQuaternion,table_3 boxHalfSize,\nbool boxIsSolid,table_3 point)"),LUA_GETBOXPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETTRIANGLETRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2=",LUA_GETTRIANGLETRIANGLEDISTANCE_COMMAND,"(\ntable_3 tri1Pt1,table_3 tri1Pt2,table_3 tri1Pt3\n,table_3 tri2Pt1,table_3 tri2Pt2,table_3 tri2Pt3)"),LUA_GETTRIANGLETRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETTRIANGLESEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2=",LUA_GETTRIANGLESEGMENTDISTANCE_COMMAND,"(\ntable_3 triPt1,table_3 triPt2,table_3 triPt3\n,table_3 segmentPt1,table_3 segmentPt2)"),LUA_GETTRIANGLESEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETTRIANGLEPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt=",LUA_GETTRIANGLEPOINTDISTANCE_COMMAND,"(\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,table_3 point)"),LUA_GETTRIANGLEPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETSEGMENTSEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2=",LUA_GETSEGMENTSEGMENTDISTANCE_COMMAND,"(\ntable_3 segment1Pt1,table_3 segment1Pt2,table_3 segment2Pt1,table_3 segment2Pt2)"),LUA_GETSEGMENTSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETSEGMENTPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt=",LUA_GETSEGMENTPOINTDISTANCE_COMMAND,"(\ntable_3 segmentPt1,table_3 segmentPt2,table_3 point)"),LUA_GETSEGMENTPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_CREATEMESH_COMMAND_PLUGIN,strConCat("number meshHandle=",LUA_CREATEMESH_COMMAND,"(table vertices,table indices,\ntable_3 meshOriginPos=nil,table_4 meshOriginQuaternion=nil,\nnumber maxTriangleEdgeLength=0.3,number maxTriangleCountInLeafObb=8)"),LUA_CREATEMESH_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_COPYMESH_COMMAND_PLUGIN,strConCat("number meshHandle=",LUA_COPYMESH_COMMAND,"(number meshHandle)"),LUA_COPYMESH_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_SCALEMESH_COMMAND_PLUGIN,strConCat("",LUA_SCALEMESH_COMMAND,"(number meshHandle,number scaleFactor)"),LUA_SCALEMESH_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHSERIALIZATIONDATA_COMMAND_PLUGIN,strConCat("string data=",LUA_GETMESHSERIALIZATIONDATA_COMMAND,"(number meshHandle)"),LUA_GETMESHSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEMESHFROMSERIALIZATIONDATA_COMMAND_PLUGIN,strConCat("number meshHandle=",LUA_CREATEMESHFROMSERIALIZATIONDATA_COMMAND,"(string data)"),LUA_CREATEMESHFROMSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_DESTROYMESH_COMMAND_PLUGIN,strConCat("",LUA_DESTROYMESH_COMMAND,"(number meshHandle)"),LUA_DESTROYMESH_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_CREATEOCTREEFROMPOINTS_COMMAND_PLUGIN,strConCat("number octreeHandle=",LUA_CREATEOCTREEFROMPOINTS_COMMAND,"(table points,\ntable_3 octreeOriginPos=nil,table_4 octreeOriginQuaternion=nil,\nnumber maxCellSize=0.05,table_3 pointColor={0,0,0},number userData=0)"),LUA_CREATEOCTREEFROMPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEOCTREEFROMCOLORPOINTS_COMMAND_PLUGIN,strConCat("number octreeHandle=",LUA_CREATEOCTREEFROMCOLORPOINTS_COMMAND,"(table points,\ntable_3 octreeOriginPos=nil,table_4 octreeOriginQuaternion=nil,\nnumber maxCellSize=0.05,table colors=nil,table userData=nil)"),LUA_CREATEOCTREEFROMCOLORPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEOCTREEFROMMESH_COMMAND_PLUGIN,strConCat("number octreeHandle=",LUA_CREATEOCTREEFROMMESH_COMMAND,"(number meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 octreeOriginPos=nil,table_4 octreeOriginQuaternion=nil,\nnumber maxCellSize=0.05,table_3 pointColor={0,0,0},number userData=0)"),LUA_CREATEOCTREEFROMMESH_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEOCTREEFROMOCTREE_COMMAND_PLUGIN,strConCat("number octreeHandle=",LUA_CREATEOCTREEFROMOCTREE_COMMAND,"(number octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 newOctreeOriginPos=nil,table_4 newOctreeOriginQuaternion=nil,\nnumber maxCellSize=0.05,table_3 pointColor={0,0,0},number userData=0)"),LUA_CREATEOCTREEFROMOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_COPYOCTREE_COMMAND_PLUGIN,strConCat("number octreeHandle=",LUA_COPYOCTREE_COMMAND,"(number octreeHandle)"),LUA_COPYOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_SCALEOCTREE_COMMAND_PLUGIN,strConCat("",LUA_SCALEOCTREE_COMMAND,"(number octreeHandle,number scaleFactor)"),LUA_SCALEOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREESERIALIZATIONDATA_COMMAND_PLUGIN,strConCat("string data=",LUA_GETOCTREESERIALIZATIONDATA_COMMAND,"(number octreeHandle)"),LUA_GETOCTREESERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEOCTREEFROMSERIALIZATIONDATA_COMMAND_PLUGIN,strConCat("number octreeHandle=",LUA_CREATEOCTREEFROMSERIALIZATIONDATA_COMMAND,"(string data)"),LUA_CREATEOCTREEFROMSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_DESTROYOCTREE_COMMAND_PLUGIN,strConCat("",LUA_DESTROYOCTREE_COMMAND,"(number octreeHandle)"),LUA_DESTROYOCTREE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEVOXELS_COMMAND_PLUGIN,strConCat("table positions,table colors,table userData",LUA_GETOCTREEVOXELS_COMMAND,"(number octreeHandle)"),LUA_GETOCTREEVOXELS_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_CREATEPTCLOUDFROMPOINTS_COMMAND_PLUGIN,strConCat("number ptcloudHandle=",LUA_CREATEPTCLOUDFROMPOINTS_COMMAND,"(table points,\ntable_3 octreeOriginPos=nil,table_4 octreeOriginQuaternion=nil,\nnumber maxCellSize=0.05,number maxPtsInCell=20,table_3 pointColor={0,0,0},number proximityTolerance=0.005)"),LUA_CREATEPTCLOUDFROMPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEPTCLOUDFROMCOLORPOINTS_COMMAND_PLUGIN,strConCat("number ptcloudHandle=",LUA_CREATEPTCLOUDFROMCOLORPOINTS_COMMAND,"(table points,\ntable_3 octreeOriginPos=nil,table_4 octreeOriginQuaternion=nil,\nnumber maxCellSize=0.05,number maxPtsInCell=20,table colors=nil,number proximityTolerance=0.005)"),LUA_CREATEPTCLOUDFROMCOLORPOINTS_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_COPYPTCLOUD_COMMAND_PLUGIN,strConCat("number ptcloudHandle=",LUA_COPYPTCLOUD_COMMAND,"(number ptcloudHandle)"),LUA_COPYPTCLOUD_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_SCALEPTCLOUD_COMMAND_PLUGIN,strConCat("",LUA_SCALEPTCLOUD_COMMAND,"(number ptcloudHandle,number scaleFactor)"),LUA_SCALEPTCLOUD_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDSERIALIZATIONDATA_COMMAND_PLUGIN,strConCat("string data=",LUA_GETPTCLOUDSERIALIZATIONDATA_COMMAND,"(number octreeHandle)"),LUA_GETPTCLOUDSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_COMMAND_PLUGIN,strConCat("number ptcloudHandle=",LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_COMMAND,"(string data)"),LUA_CREATEPTCLOUDFROMSERIALIZATIONDATA_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_DESTROYPTCLOUD_COMMAND_PLUGIN,strConCat("",LUA_DESTROYPTCLOUD_COMMAND,"(number ptcloudHandle)"),LUA_DESTROYPTCLOUD_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDPOINTS_COMMAND_PLUGIN,strConCat("table points,table colors",LUA_GETPTCLOUDPOINTS_COMMAND,"(number ptcloudHandle,number subsetProportion=1.0)"),LUA_GETPTCLOUDPOINTS_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETTRANSFORMEDPOINTS_COMMAND_PLUGIN,strConCat("table transformedPoints",LUA_GETTRANSFORMEDPOINTS_COMMAND,"(table points,table_3 position,table_4 quaternion)"),LUA_GETTRANSFORMEDPOINTS_CALLBACK);


    return(4);
}

SIM_DLLEXPORT void simEnd()
{
}

SIM_DLLEXPORT void* simMessage(int message,int*,void*,int*)
{
    if (message==sim_message_eventcallback_lastinstancepass)
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
    return(nullptr);
}

SIM_DLLEXPORT void geomPlugin_releaseBuffer(void* buff)
{
    delete[] ((char*)buff);
}

SIM_DLLEXPORT void* geomPlugin_createMesh(const float* vertices,int verticesSize,const int* indices,int indicesSize,const float meshOrigin[7],float triangleEdgeMaxLength,int maxTrianglesInBoundingBox)
{
    C7Vector tr;
    tr.setIdentity();
    if (meshOrigin!=nullptr)
        tr.setInternalData(meshOrigin);
    void* retVal=geom_createMesh(vertices,verticesSize,indices,indicesSize,&tr,triangleEdgeMaxLength,maxTrianglesInBoundingBox);
    return(retVal);
}

SIM_DLLEXPORT void* geomPlugin_copyMesh(const void* meshObbStruct)
{
    void* retVal=geom_copyMesh((const CObbStruct*)meshObbStruct);
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_scaleMesh(void* meshObbStruct,float scalingFactor)
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

SIM_DLLEXPORT float geomPlugin_getMeshRootObbVolume(const void* meshObbStruct)
{
    return(geom_getMeshRootObbVolume((const CObbStruct*)meshObbStruct));
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const float* points,int pointCnt,const float octreeOrigin[7],float cellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setInternalData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromPoints(points,pointCnt,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const float* points,int pointCnt,const float octreeOrigin[7],float cellS,const unsigned char* rgbData,const unsigned int* usrData)
{
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setInternalData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromColorPoints(points,pointCnt,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const void* meshObbStruct,const float meshTransformation[7],const float octreeOrigin[7],float cellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector _meshTransformation;
    _meshTransformation.setInternalData(meshTransformation);
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setInternalData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromMesh((const CObbStruct*)meshObbStruct,_meshTransformation,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const void* otherOctreeStruct,const float otherOctreeTransformation[7],const float newOctreeOrigin[7],float newOctreeCellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector _otherOctreeTransformation;
    _otherOctreeTransformation.setInternalData(otherOctreeTransformation);
    C7Vector tr;
    tr.setIdentity();
    if (newOctreeOrigin!=nullptr)
        tr.setInternalData(newOctreeOrigin);
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
SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,float f)
{
    geom_scaleOctree((COcStruct*)ocStruct,f);
}
SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct)
{
    geom_destroyOctree((COcStruct*)ocStruct);
}
SIM_DLLEXPORT float* geomPlugin_getOctreeVoxelData(const void* ocStruct,int* voxelCount)
{ // voxel position + rgb
    std::vector<float> data;
    geom_getOctreeVoxelData((const COcStruct*)ocStruct,data,nullptr);
    float* retVal=new float[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    voxelCount[0]=((int)data.size())/6;
    return(retVal);
}
SIM_DLLEXPORT unsigned int* geomPlugin_getOctreeUserData(const void* ocStruct,int* voxelCount)
{
    std::vector<float> data;
    std::vector<unsigned int> usrData;
    geom_getOctreeVoxelData((const COcStruct*)ocStruct,data,&usrData);
    unsigned int* retVal=new unsigned int[usrData.size()];
    for (size_t i=0;i<usrData.size();i++)
        retVal[i]=usrData[i];
    voxelCount[0]=(int)usrData.size();
    return(retVal);
}
SIM_DLLEXPORT float* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* pointCount)
{
    std::vector<float> data;
    geom_getOctreeCornersFromOctree((const COcStruct*)ocStruct,data);
    float* retVal=new float[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/3;
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const float octreeTransformation[7],const float* points,int pointCnt,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    geom_insertPointsIntoOctree((COcStruct*)ocStruct,tr,points,pointCnt,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const float octreeTransformation[7],const float* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    geom_insertColorPointsIntoOctree((COcStruct*)ocStruct,tr,points,pointCnt,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const float octreeTransformation[7],const void* obbStruct,const float meshTransformation[7],const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C7Vector meshTr;
    meshTr.setInternalData(meshTransformation);
    geom_insertMeshIntoOctree((COcStruct*)ocStruct,tr,(const CObbStruct*)obbStruct,meshTr,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7],const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    geom_insertOctreeIntoOctree((COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,rgbData,usrData);
}
SIM_DLLEXPORT bool geomPlugin_removePointsFromOctree(void* ocStruct,const float octreeTransformation[7],const float* points,int pointCnt)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    bool retVal=geom_removePointsFromOctree((COcStruct*)ocStruct,tr,points,pointCnt);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeMeshFromOctree(void* ocStruct,const float octreeTransformation[7],const void* obbStruct,const float meshTransformation[7])
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C7Vector meshTr;
    meshTr.setInternalData(meshTransformation);
    bool retVal=geom_removeMeshFromOctree((COcStruct*)ocStruct,tr,(const CObbStruct*)obbStruct,meshTr);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromOctree(void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7])
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    bool retVal=geom_removeOctreeFromOctree((COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2);
    return(retVal);
}



SIM_DLLEXPORT void* geomPlugin_createPtcloudFromPoints(const float* points,int pointCnt,const float ptcloudOrigin[7],float cellS,int maxPointCnt,const unsigned char rgbData[3],float proximityTol)
{
    C7Vector tr;
    tr.setIdentity();
    if (ptcloudOrigin!=nullptr)
        tr.setInternalData(ptcloudOrigin);
    void* retVal=geom_createPtcloudFromPoints(points,pointCnt,&tr,cellS,maxPointCnt,rgbData,proximityTol);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromColorPoints(const float* points,int pointCnt,const float ptcloudOrigin[7],float cellS,int maxPointCnt,const unsigned char* rgbData,float proximityTol)
{
    C7Vector tr;
    tr.setIdentity();
    if (ptcloudOrigin!=nullptr)
        tr.setInternalData(ptcloudOrigin);
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
SIM_DLLEXPORT void geomPlugin_scalePtcloud(void* pcStruct,float f)
{
    geom_scalePtcloud((CPcStruct*)pcStruct,f);
}
SIM_DLLEXPORT void geomPlugin_destroyPtcloud(void* pcStruct)
{
    geom_destroyPtcloud((CPcStruct*)pcStruct);
}
SIM_DLLEXPORT float* geomPlugin_getPtcloudPoints(const void* pcStruct,int* pointCount,float prop)
{
    std::vector<float> data;
    geom_getPtcloudPoints((const CPcStruct*)pcStruct,data,prop);
    float* retVal=new float[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/6;
    return(retVal);
}
SIM_DLLEXPORT float* geomPlugin_getPtcloudOctreeCorners(const void* pcStruct,int* pointCount)
{
    std::vector<float> data;
    geom_getPtcloudOctreeCorners((const CPcStruct*)pcStruct,data);
    float* retVal=new float[data.size()];
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

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,const unsigned char rgbData[3],float proximityTol)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    geom_insertPointsIntoPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,rgbData,proximityTol);
}
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,const unsigned char* rgbData,float proximityTol)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    geom_insertColorPointsIntoPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,rgbData,proximityTol);
}
SIM_DLLEXPORT bool geomPlugin_removePointsFromPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,float proximityTol,int* countRemoved)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    bool retVal=geom_removePointsFromPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,proximityTol,countRemoved);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromPtcloud(void* pcStruct,const float ptcloudTransformation[7],const void* ocStruct,const float octreeTransformation[7],int* countRemoved)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    C7Vector octreeTr;
    octreeTr.setInternalData(octreeTransformation);
    bool retVal=geom_removeOctreeFromPtcloud((CPcStruct*)pcStruct,tr,(const COcStruct*)ocStruct,octreeTr,countRemoved);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_intersectPointsWithPtcloud(void* pcStruct,const float ptcloudTransformation[7],const float* points,int pointCnt,float proximityTol)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    bool retVal=geom_intersectPointsWithPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,proximityTol);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getMeshMeshCollision(const void* mesh1ObbStruct,const float mesh1Transformation[7],const void* mesh2ObbStruct,const float mesh2Transformation[7],float** intersections,int* intersectionsSize,int* mesh1Caching,int* mesh2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(mesh1Transformation);
    C7Vector tr2;
    tr2.setInternalData(mesh2Transformation);
    std::vector<float> _intersections;
    std::vector<float>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshMeshCollision((const CObbStruct*)mesh1ObbStruct,tr1,(const CObbStruct*)mesh2ObbStruct,tr2,_ints,mesh1Caching,mesh2Caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new float[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeCollision(const void* meshObbStruct,const float meshTransformation[7],const void* ocStruct,const float octreeTransformation[7],int* meshCaching,unsigned long long int* ocCaching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C7Vector tr2;
    tr2.setInternalData(octreeTransformation);
    bool retVal=geom_getMeshOctreeCollision((const CObbStruct*)meshObbStruct,tr1,(const COcStruct*)ocStruct,tr2,meshCaching,ocCaching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleCollision(const void* meshObbStruct,const float meshTransformation[7],const float p[3],const float v[3],const float w[3],float** intersections,int* intersectionsSize,int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    std::vector<float> _intersections;
    std::vector<float>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshTriangleCollision((const CObbStruct*)meshObbStruct,tr1,_p,_v,_w,_ints,caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new float[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentCollision(const void* meshObbStruct,const float meshTransformation[7],const float segmentExtremity[3],const float segmentVector[3],float** intersections,int* intersectionsSize,int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _segmentExtremity(segmentExtremity);
    C3Vector _segmentVector(segmentVector);
    std::vector<float> _intersections;
    std::vector<float>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshSegmentCollision((const CObbStruct*)meshObbStruct,tr1,_segmentExtremity,_segmentVector,_ints,caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new float[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    bool retVal=geom_getOctreeOctreeCollision((const COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,oc1Caching,oc2Caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudCollision(const void* ocStruct,const float octreeTransformation[7],const void* pcStruct,const float ptcloudTransformation[7],unsigned long long int* ocCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C7Vector tr2;
    tr2.setInternalData(ptcloudTransformation);
    bool retVal=geom_getOctreePtcloudCollision((const COcStruct*)ocStruct,tr1,(const CPcStruct*)pcStruct,tr2,ocCaching,pcCaching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleCollision(const void* ocStruct,const float octreeTransformation[7],const float p[3],const float v[3],const float w[3],unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    bool retVal=geom_getOctreeTriangleCollision((const COcStruct*)ocStruct,tr1,_p,_v,_w,caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentCollision(const void* ocStruct,const float octreeTransformation[7],const float segmentExtremity[3],const float segmentVector[3],unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C3Vector _segmentExtremity(segmentExtremity);
    C3Vector _segmentVector(segmentVector);
    bool retVal=geom_getOctreeSegmentCollision((const COcStruct*)ocStruct,tr1,_segmentExtremity,_segmentVector,caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePointsCollision(const void* ocStruct,const float octreeTransformation[7],const float* points,int pointCount)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    bool retVal=geom_getOctreePointsCollision((const COcStruct*)ocStruct,tr1,points,pointCount);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreePointCollision(const void* ocStruct,const float octreeTransformation[7],const float point[3],unsigned int* usrData,unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C3Vector _point(point);
    bool retVal=geom_getOctreePointCollision((const COcStruct*)ocStruct,tr1,_point,usrData,caching);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getBoxBoxCollision(const float box1Transformation[7],const float box1HalfSize[3],const float box2Transformation[7],const float box2HalfSize[3],bool boxesAreSolid)
{
    C7Vector tr1;
    tr1.setInternalData(box1Transformation);
    C7Vector tr2;
    tr2.setInternalData(box2Transformation);
    C3Vector b1hs(box1HalfSize);
    C3Vector b2hs(box2HalfSize);
    bool retVal=geom_getBoxBoxCollision(tr1,b1hs,tr2,b2hs,boxesAreSolid);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleCollision(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float p[3],const float v[3],const float w[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    bool retVal=geom_getBoxTriangleCollision(tr,bhs,boxIsSolid,_p,_v,_w);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentCollision(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float segmentEndPoint[3],const float segmentVector[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    bool retVal=geom_getBoxSegmentCollision(tr,bhs,boxIsSolid,_segmentEndPoint,_segmentVector);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxPointCollision(const float boxTransformation[7],const float boxHalfSize[3],const float point[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _point(point);
    bool retVal=geom_getBoxPointCollision(tr,bhs,_point);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleCollision(const float p1[3],const float v1[3],const float w1[3],const float p2[3],const float v2[3],const float w2[3],float** intersections,int* intersectionsSize)
{
    C3Vector _p1(p1);
    C3Vector _v1(v1);
    C3Vector _w1(w1);
    C3Vector _p2(p2);
    C3Vector _v2(v2);
    C3Vector _w2(w2);
    std::vector<float> _intersections;
    std::vector<float>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getTriangleTriangleCollision(_p1,_v1,_w1,_p2,_v2,_w2,_ints);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new float[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentCollision(const float p[3],const float v[3],const float w[3],const float segmentEndPoint[3],const float segmentVector[3],float** intersections,int* intersectionsSize)
{
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    std::vector<float> _intersections;
    std::vector<float>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getTriangleSegmentCollision(_p,_v,_w,_segmentEndPoint,_segmentVector,_ints);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new float[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getMeshMeshDistanceIfSmaller(const void* mesh1ObbStruct,const float mesh1Transformation[7],const void* mesh2ObbStruct,const float mesh2Transformation[7],float* dist,float minDistSegPt1[3],float minDistSegPt2[3],int* mesh1Caching,int* mesh2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(mesh1Transformation);
    C7Vector tr2;
    tr2.setInternalData(mesh2Transformation);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getMeshMeshDistanceIfSmaller((const CObbStruct*)mesh1ObbStruct,tr1,(const CObbStruct*)mesh2ObbStruct,tr2,dist[0],&_minDistSegPt1,&_minDistSegPt2,mesh1Caching,mesh2Caching);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getInternalData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getInternalData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const void* ocStruct,const float octreeTransformation[7],float* dist,float meshMinDistPt[3],float ocMinDistPt[3],int* meshCaching,unsigned long long int* ocCaching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C7Vector tr2;
    tr2.setInternalData(octreeTransformation);
    C3Vector _meshMinDistPt;
    C3Vector _ocMinDistPt;
    bool retVal=geom_getMeshOctreeDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,(const COcStruct*)ocStruct,tr2,dist[0],&_meshMinDistPt,&_ocMinDistPt,meshCaching,ocCaching);
    if (retVal)
    {
        if (meshMinDistPt!=nullptr)
            _meshMinDistPt.getInternalData(meshMinDistPt);
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getInternalData(ocMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshPtcloudDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const void* pcStruct,const float pcTransformation[7],float* dist,float meshMinDistPt[3],float pcMinDistPt[3],int* meshCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C7Vector tr2;
    tr2.setInternalData(pcTransformation);
    C3Vector _meshMinDistPt;
    C3Vector _pcMinDistPt;
    bool retVal=geom_getMeshPtcloudDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,(const CPcStruct*)pcStruct,tr2,dist[0],&_meshMinDistPt,&_pcMinDistPt,meshCaching,pcCaching);
    if (retVal)
    {
        if (meshMinDistPt!=nullptr)
            _meshMinDistPt.getInternalData(meshMinDistPt);
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getInternalData(pcMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const float p[3],const float v[3],const float w[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3],int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getMeshTriangleDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,_p,_v,_w,dist[0],&_minDistSegPt1,&_minDistSegPt2,caching);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getInternalData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getInternalData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3],int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    bool retVal=geom_getMeshSegmentDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,_segmentEndPoint,_segmentVector,dist[0],&_minDistSegPt1,&_minDistSegPt2,caching);
    if (retVal)
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.getInternalData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getInternalData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshPointDistanceIfSmaller(const void* meshObbStruct,const float meshTransformation[7],const float point[3],float* dist,float minDistSegPt[3],int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _point(point);
    C3Vector _minDistSegPt;
    bool retVal=geom_getMeshPointDistanceIfSmaller((const CObbStruct*)meshObbStruct,tr1,_point,dist[0],&_minDistSegPt,caching);
    if (retVal)
    {
        if (minDistSegPt!=nullptr)
            _minDistSegPt.getInternalData(minDistSegPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const float octree1Transformation[7],const void* oc2Struct,const float octree2Transformation[7],float* dist,float oc1MinDistPt[3],float oc2MinDistPt[3],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    C3Vector _oc1MinDistPt;
    C3Vector _oc2MinDistPt;
    bool retVal=geom_getOctreeOctreeDistanceIfSmaller((const COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,dist[0],&_oc1MinDistPt,&_oc2MinDistPt,oc1Caching,oc2Caching);
    if (retVal)
    {
        if (oc1MinDistPt!=nullptr)
            _oc1MinDistPt.getInternalData(oc1MinDistPt);
        if (oc2MinDistPt!=nullptr)
            _oc2MinDistPt.getInternalData(oc2MinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const void* pcStruct,const float pcTransformation[7],float* dist,float ocMinDistPt[3],float pcMinDistPt[3],unsigned long long int* ocCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C7Vector tr2;
    tr2.setInternalData(pcTransformation);
    C3Vector _ocMinDistPt;
    C3Vector _pcMinDistPt;
    bool retVal=geom_getOctreePtcloudDistanceIfSmaller((const COcStruct*)ocStruct,tr1,(const CPcStruct*)pcStruct,tr2,dist[0],&_ocMinDistPt,&_pcMinDistPt,ocCaching,pcCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getInternalData(ocMinDistPt);
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getInternalData(pcMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const float p[3],const float v[3],const float w[3],float* dist,float ocMinDistPt[3],float triMinDistPt[3],unsigned long long int* ocCaching)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _ocMinDistPt;
    C3Vector _triMinDistPt;
    bool retVal=geom_getOctreeTriangleDistanceIfSmaller((const COcStruct*)ocStruct,tr,_p,_v,_w,dist[0],&_ocMinDistPt,&_triMinDistPt,ocCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getInternalData(ocMinDistPt);
        if (triMinDistPt!=nullptr)
            _triMinDistPt.getInternalData(triMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float ocMinDistPt[3],float segMinDistPt[3],unsigned long long int* ocCaching)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _ocMinDistPt;
    C3Vector _segMinDistPt;
    bool retVal=geom_getOctreeSegmentDistanceIfSmaller((const COcStruct*)ocStruct,tr,_segmentEndPoint,_segmentVector,dist[0],&_ocMinDistPt,&_segMinDistPt,ocCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getInternalData(ocMinDistPt);
        if (segMinDistPt!=nullptr)
            _segMinDistPt.getInternalData(segMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const float octreeTransformation[7],const float point[3],float* dist,float ocMinDistPt[3],unsigned long long int* ocCaching)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C3Vector _point(point);
    C3Vector _ocMinDistPt;
    bool retVal=geom_getOctreePointDistanceIfSmaller((const COcStruct*)ocStruct,tr,_point,dist[0],&_ocMinDistPt,ocCaching);
    if (retVal)
    {
        if (ocMinDistPt!=nullptr)
            _ocMinDistPt.getInternalData(ocMinDistPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getPtcloudPtcloudDistanceIfSmaller(const void* pc1Struct,const float pc1Transformation[7],const void* pc2Struct,const float pc2Transformation[7],float* dist,float* pc1MinDistPt,float* pc2MinDistPt,unsigned long long int* pc1Caching,unsigned long long int* pc2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(pc1Transformation);
    C7Vector tr2;
    tr2.setInternalData(pc2Transformation);
    C3Vector _pc1MinDistPt;
    C3Vector _pc2MinDistPt;
    bool retVal=geom_getPtcloudPtcloudDistanceIfSmaller((const CPcStruct*)pc1Struct,tr1,(const CPcStruct*)pc2Struct,tr2,dist[0],&_pc1MinDistPt,&_pc2MinDistPt,pc1Caching,pc2Caching);
    if (retVal)
    {
        if (pc1MinDistPt!=nullptr)
            _pc1MinDistPt.getInternalData(pc1MinDistPt);
        if (pc2MinDistPt!=nullptr)
            _pc2MinDistPt.getInternalData(pc2MinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getPtcloudTriangleDistanceIfSmaller(const void* pcStruct,const float pcTransformation[7],const float p[3],const float v[3],const float w[3],float* dist,float* pcMinDistPt,float* triMinDistPt,unsigned long long int* pcCaching)
{
    C7Vector tr;
    tr.setInternalData(pcTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _pcMinDistPt;
    C3Vector _triMinDistPt;
    bool retVal=geom_getPtcloudTriangleDistanceIfSmaller((const CPcStruct*)pcStruct,tr,_p,_v,_w,dist[0],&_pcMinDistPt,&_triMinDistPt,pcCaching);
    if (retVal)
    {
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getInternalData(pcMinDistPt);
        if (triMinDistPt!=nullptr)
            _triMinDistPt.getInternalData(triMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getPtcloudSegmentDistanceIfSmaller(const void* pcStruct,const float pcTransformation[7],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float* pcMinDistPt,float* segMinDistPt,unsigned long long int* pcCaching)
{
    C7Vector tr;
    tr.setInternalData(pcTransformation);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _pcMinDistPt;
    C3Vector _segMinDistPt;
    bool retVal=geom_getPtcloudSegmentDistanceIfSmaller((const CPcStruct*)pcStruct,tr,_segmentEndPoint,_segmentVector,dist[0],&_pcMinDistPt,&_segMinDistPt,pcCaching);
    if (retVal)
    {
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getInternalData(pcMinDistPt);
        if (segMinDistPt!=nullptr)
            _segMinDistPt.getInternalData(segMinDistPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getPtcloudPointDistanceIfSmaller(const void* pcStruct,const float pcTransformation[7],const float point[3],float* dist,float* pcMinDistPt,unsigned long long int* pcCaching)
{
    C7Vector tr;
    tr.setInternalData(pcTransformation);
    C3Vector _point(point);
    C3Vector _pcMinDistPt;
    bool retVal=geom_getPtcloudPointDistanceIfSmaller((const CPcStruct*)pcStruct,tr,_point,dist[0],&_pcMinDistPt,pcCaching);
    if (retVal)
    {
        if (pcMinDistPt!=nullptr)
            _pcMinDistPt.getInternalData(pcMinDistPt);
    }
    return(retVal);
}

SIM_DLLEXPORT float geomPlugin_getApproxBoxBoxDistance(const float box1Transformation[7],const float box1HalfSize[3],const float box2Transformation[7],const float box2HalfSize[3])
{
    C7Vector tr1;
    tr1.setInternalData(box1Transformation);
    C7Vector tr2;
    tr2.setInternalData(box2Transformation);
    C3Vector _b1hs(box1HalfSize);
    C3Vector _b2hs(box2HalfSize);
    float retVal=geom_getApproxBoxBoxDistance(tr1,_b1hs,tr2,_b2hs);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getBoxBoxDistanceIfSmaller(const float box1Transformation[7],const float box1HalfSize[3],const float box2Transformation[7],const float box2HalfSize[3],bool boxesAreSolid,float* dist,float distSegPt1[3],float distSegPt2[3])
{
    C7Vector tr1;
    tr1.setInternalData(box1Transformation);
    C7Vector tr2;
    tr2.setInternalData(box2Transformation);
    C3Vector _b1hs(box1HalfSize);
    C3Vector _b2hs(box2HalfSize);
    C3Vector _distSegPt1;
    C3Vector _distSegPt2;
    bool retVal=geom_getBoxBoxDistanceIfSmaller(tr1,_b1hs,tr2,_b2hs,boxesAreSolid,dist[0],&_distSegPt1,&_distSegPt2);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getInternalData(distSegPt1);
        if (distSegPt2!=nullptr)
            _distSegPt2.getInternalData(distSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleDistanceIfSmaller(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float p[3],const float v[3],const float w[3],float* dist,float distSegPt1[3],float distSegPt2[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
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
            _distSegPt1.getInternalData(distSegPt1);
        if (distSegPt2!=nullptr)
            _distSegPt2.getInternalData(distSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentDistanceIfSmaller(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float segmentEndPoint[3],const float segmentVector[3],float* dist,float distSegPt1[3],float distSegPt2[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector _bhs(boxHalfSize);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _distSegPt1;
    C3Vector _distSegPt2;
    bool retVal=geom_getBoxSegmentDistanceIfSmaller(tr,_bhs,boxIsSolid,_segmentEndPoint,_segmentVector,dist[0],&_distSegPt1,&_distSegPt2);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getInternalData(distSegPt1);
        if (distSegPt2!=nullptr)
            _distSegPt2.getInternalData(distSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxPointDistanceIfSmaller(const float boxTransformation[7],const float boxHalfSize[3],bool boxIsSolid,const float point[3],float* dist,float distSegPt1[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector _bhs(boxHalfSize);
    C3Vector _point(point);
    C3Vector _distSegPt1;
    bool retVal=geom_getBoxPointDistanceIfSmaller(tr,_bhs,boxIsSolid,_point,dist[0],&_distSegPt1);
    if (retVal)
    {
        if (distSegPt1!=nullptr)
            _distSegPt1.getInternalData(distSegPt1);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleDistanceIfSmaller(const float p1[3],const float v1[3],const float w1[3],const float p2[3],const float v2[3],const float w2[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3])
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
            _minDistSegPt1.getInternalData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getInternalData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentDistanceIfSmaller(const float p[3],const float v[3],const float w[3],const float segmentEndPoint[3],const float segmentVector[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3])
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
            _minDistSegPt1.getInternalData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getInternalData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTrianglePointDistanceIfSmaller(const float p[3],const float v[3],const float w[3],const float point[3],float* dist,float minDistSegPt[3])
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
            _minDistSegPt.getInternalData(minDistSegPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getSegmentSegmentDistanceIfSmaller(const float segment1EndPoint[3],const float segment1Vector[3],const float segment2EndPoint[3],const float segment2Vector[3],float* dist,float minDistSegPt1[3],float minDistSegPt2[3])
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
            _minDistSegPt1.getInternalData(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.getInternalData(minDistSegPt2);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getSegmentPointDistanceIfSmaller(const float segmentEndPoint[3],const float segmentVector[3],const float point[3],float* dist,float minDistSegPt[3])
{
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _point(point);
    C3Vector _minDistSegPt;
    bool retVal=geom_getSegmentPointDistanceIfSmaller(_segmentEndPoint,_segmentVector,_point,dist[0],&_minDistSegPt);
    if (retVal)
    {
        if (minDistSegPt!=nullptr)
            _minDistSegPt.getInternalData(minDistSegPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectMeshIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const void* obbStruct,const float meshTransformationRelative[7],float* dist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3])
{   // sensor is at the origin. meshTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setInternalData(meshTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_volumeSensorDetectMeshIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,(const CObbStruct*)obbStruct,tr,dist[0],fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getInternalData(detectPt);
        if (triN!=nullptr)
            _triN.getInternalData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectOctreeIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const void* ocStruct,const float octreeTransformationRelative[7],float* dist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3])
{   // sensor is at the origin. octreeTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setInternalData(octreeTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_volumeSensorDetectOctreeIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,(const COcStruct*)ocStruct,tr,dist[0],fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getInternalData(detectPt);
        if (triN!=nullptr)
            _triN.getInternalData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectPtcloudIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const void* pcStruct,const float ptcloudTransformationRelative[7],float* dist,bool fast,float detectPt[3])
{   // sensor is at the origin. ptcloudTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setInternalData(ptcloudTransformationRelative);
    C3Vector _detectPt;
    bool retVal=geom_volumeSensorDetectPtcloudIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,(const CPcStruct*)pcStruct,tr,dist[0],fast,&_detectPt);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getInternalData(detectPt);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectTriangleIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const float p[3],const float v[3],const float w[3],float* dist,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3])
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
            _detectPt.getInternalData(detectPt);
        if (triN!=nullptr)
            _triN.getInternalData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectSegmentIfSmaller(const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,const float segmentEndPoint[3],const float segmentVector[3],float* dist,float maxAngle,float detectPt[3])
{   // sensor is at the origin. segment is relative to the sensor
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    C3Vector _detectPt;
    bool retVal=geom_volumeSensorDetectSegmentIfSmaller(planesIn,planesInSize,planesOut,planesOutSize,_segmentEndPoint,_segmentVector,dist[0],maxAngle,&_detectPt);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getInternalData(detectPt);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_raySensorDetectMeshIfSmaller(const float rayStart[3],const float rayVect[3],const void* obbStruct,const float meshTransformationRelative[7],float* dist,float forbiddenDist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3],bool* forbiddenDistTouched)
{   // sensor is at the origin. meshTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setInternalData(meshTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_raySensorDetectMeshIfSmaller(C3Vector(rayStart),C3Vector(rayVect),(const CObbStruct*)obbStruct,tr,dist[0],forbiddenDist,fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN,forbiddenDistTouched);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getInternalData(detectPt);
        if (triN!=nullptr)
            _triN.getInternalData(triN);
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_raySensorDetectOctreeIfSmaller(const float rayStart[3],const float rayVect[3],const void* ocStruct,const float octreeTransformationRelative[7],float* dist,float forbiddenDist,bool fast,bool frontDetection,bool backDetection,float maxAngle,float detectPt[3],float triN[3],bool* forbiddenDistTouched)
{   // sensor is at the origin. octreeTransformationRelative is relative to the sensor
    C7Vector tr;
    tr.setInternalData(octreeTransformationRelative);
    C3Vector _detectPt;
    C3Vector _triN;
    bool retVal=geom_raySensorDetectOctreeIfSmaller(C3Vector(rayStart),C3Vector(rayVect),(const COcStruct*)ocStruct,tr,dist[0],forbiddenDist,fast,frontDetection,backDetection,maxAngle,&_detectPt,&_triN,forbiddenDistTouched);
    if (retVal)
    {
        if (detectPt!=nullptr)
            _detectPt.getInternalData(detectPt);
        if (triN!=nullptr)
            _triN.getInternalData(triN);
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_isPointInVolume(const float* planesIn,int planesInSize,const float point[3])
{
    bool retVal=geom_isPointInVolume(planesIn,planesInSize,C3Vector(point));
    return(retVal);
}

/*







































SIM_DLLEXPORT char geomPlugin_getBoxBoxCollision(const float* box1,const float* box1Hs,const float* box2,const float* box2Hs)
{
    C4X4Matrix _box1;
    _box1.copyFromInterface(box1);
    C4X4Matrix _box2;
    _box2.copyFromInterface(box2);
    C3Vector _box1Hs(box1Hs);
    C3Vector _box2Hs(box2Hs);
    if (CCalcUtils::doCollide_box_box(_box1,_box1Hs,_box2,_box2Hs,true))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getBoxPointCollision(const float* box,const float* boxHs,const float* pt)
{
    C4X4Matrix _box;
    _box.copyFromInterface(box);
    _box.inverse();
    C3Vector _pt(pt);
    _pt*=_box;
    if (fabsf(_pt(0))>boxHs[0])
        return(0);
    if (fabsf(_pt(1))>boxHs[1])
        return(0);
    if (fabsf(_pt(2))>boxHs[2])
        return(0);
    return(1);
}

SIM_DLLEXPORT int geomPlugin_getTriangleTriangleCollision(const float* p1,const float* v1,const float* w1,const float* p2,const float* v2,const float* w2,float* dPt1,float* dPt2,char getIntersections)
{
    C3Vector _p1(p1);
    C3Vector _v1(v1);
    C3Vector _w1(w1);
    C3Vector _p2(p2);
    C3Vector _v2(v2);
    C3Vector _w2(w2);
    if (getIntersections!=0)
    {
        std::vector<float> intersections;
        if (CCalcUtils::doCollide_tri_tri(_p1,_v1,_w1,-1,_p2,_v2,_w2,-1,&intersections,nullptr,nullptr))
        {
            if (intersections.size()==6)
            {
                dPt1[0]=intersections[0];
                dPt1[1]=intersections[1];
                dPt1[2]=intersections[2];
                dPt2[0]=intersections[3];
                dPt2[1]=intersections[4];
                dPt2[2]=intersections[5];
                return(1);
            }
            return(2);
        }
    }
    else
    {
        if (CCalcUtils::doCollide_tri_tri(_p1,_v1,_w1,-1,_p2,_v2,_w2,-1,nullptr,nullptr,nullptr))
            return(2);
    }
    return(0);
}

SIM_DLLEXPORT float geomPlugin_getBoxBoxDistance(const float* box1,const float* box1Hs,const float* box2,const float* box2Hs)
{
    C4X4Matrix _box1;
    _box1.copyFromInterface(box1);
    C3Vector _box1Hs(box1Hs);
    C4X4Matrix _box2;
    _box2.copyFromInterface(box2);
    C3Vector _box2Hs(box2Hs);
    float d=SIM_MAX_FLOAT;
    CCalcUtils::getDistance_box_box(_box1,_box1Hs,_box2,_box2Hs,true,d,nullptr,nullptr);
    return(d);
}

SIM_DLLEXPORT float geomPlugin_getBoxPointDistance(const float* box,const float* boxHs,const float* pt)
{
    C4X4Matrix _box;
    _box.copyFromInterface(box);
    C3Vector _boxHs(boxHs);
    C3Vector _pt(pt);
    float dist=SIM_MAX_FLOAT;
    CCalcUtils::getDistance_box_pt(_box,_boxHs,true,_pt,dist,nullptr,nullptr);
    return(dist);
}

SIM_DLLEXPORT float geomPlugin_getApproxBoxBoxDistance(const float* box1,const float* box1Hs,const float* box2,const float* box2Hs)
{
    C4X4Matrix _box1;
    _box1.copyFromInterface(box1);
    C3Vector _box1Hs(box1Hs);
    C4X4Matrix _box2;
    _box2.copyFromInterface(box2);
    C3Vector _box2Hs(box2Hs);
    return(CCalcUtils::getApproxDistance_box_box(_box1,_box1Hs,_box2,_box2Hs));
}

SIM_DLLEXPORT char geomPlugin_getTriangleTriangleDistanceIfSmaller(const float* p1,const float* v1,const float* w1,const float* p2,const float* v2,const float* w2,float* dist,float* minDistSegPt1,float* minDistSegPt2)
{
    C3Vector _p1(p1);
    C3Vector _v1(v1);
    C3Vector _w1(w1);
    C3Vector _p2(p2);
    C3Vector _v2(v2);
    C3Vector _w2(w2);
    C3Vector _minDistSegPt1,_minDistSegPt2;
    if (CCalcUtils::getDistance_tri_tri(_p1,_v1,_w1,_p2,_v2,_w2,dist[0],&_minDistSegPt1,&_minDistSegPt2))
    {
        _minDistSegPt1.copyTo(minDistSegPt1);
        _minDistSegPt2.copyTo(minDistSegPt2);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getTrianglePointDistanceIfSmaller(const float* p,const float* v,const float* w,const float* pt,float* dist,float* minDistSegPt)
{
    C3Vector _minDistSegPt;
    if (CCalcUtils::getDistance_tri_pt(C3Vector(p),C3Vector(v),C3Vector(w),C3Vector(pt),dist[0],&_minDistSegPt))
    {
        if (minDistSegPt!=nullptr)
            _minDistSegPt.copyTo(minDistSegPt);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getCubeTriangleDistanceIfSmaller(float cubeS,const float* p,const float* v,const float* w,float* dist,float* minDistSegPt1,float* minDistSegPt2)
{
    C3Vector _minDistSegPt1;
    C3Vector _minDistSegPt2;
    if (CCalcUtils::getDistance_cell_tri(cubeS*0.5f,true,C3Vector(p),C3Vector(v),C3Vector(w),dist[0],&_minDistSegPt1,&_minDistSegPt2))
    {
        if (minDistSegPt1!=nullptr)
            _minDistSegPt1.copyTo(minDistSegPt1);
        if (minDistSegPt2!=nullptr)
            _minDistSegPt2.copyTo(minDistSegPt2);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_isPointTouchingVolume(const float* pt,const float* planesIn,int planesInS)
{
    C3Vector _pt(pt);
    CVolumePlanes _planesIn(planesIn,planesInS);
    return(CCalcUtils::isPointInVolume(_planesIn,_pt));
}

SIM_DLLEXPORT char geomPlugin_isPointInsideVolume1AndOutsideVolume2(const float* pt,const float* planesIn,int planesInS,const float* planesOut,int planesOutS)
{
    C3Vector _pt(pt);
    CVolumePlanes _planesIn(planesIn,planesInS);
    CVolumePlanes _planesOut(planesOut,planesOutS);
    if (_planesIn.size()>0)
    {
        if (!CCalcUtils::isPointInVolume(_planesIn,_pt))
            return(0);
    }
    if (CCalcUtils::isPointInVolume(_planesOut,_pt))
        return(0);
    return(1);
}

SIM_DLLEXPORT void* geomPlugin_createMesh(const float* vertices,int verticesSize,const int* indices,int indicesSize,float triSize,int triCnt)
{
    CObbStruct* newObbStruct=CObbStruct::copyObbStructFromExisting(vertices,verticesSize,indices,indicesSize,triSize,triCnt);
    if (newObbStruct==nullptr)
        newObbStruct=new CObbStruct(vertices,verticesSize,indices,indicesSize,triSize,triCnt);
    CObbStruct::addObbStruct(newObbStruct);
    return(newObbStruct);
}

SIM_DLLEXPORT void* geomPlugin_copyMesh(const void* obbStruct)
{
    CObbStruct* newObbStruct=((const CObbStruct*)obbStruct)->copyYourself();
    CObbStruct::addObbStruct(newObbStruct);
    return(newObbStruct);
}

SIM_DLLEXPORT void geomPlugin_scaleMesh(void* obbStruct,float f)
{
    ((CObbStruct*)obbStruct)->scaleYourself(f);
}

SIM_DLLEXPORT unsigned char* geomPlugin_getMeshSerializationData(const void* obbStruct,int* dataSize)
{
    return(((const CObbStruct*)obbStruct)->serialize(dataSize[0]));
}

SIM_DLLEXPORT void* geomPlugin_getMeshFromSerializationData(const unsigned char* data)
{
    CObbStruct* newObbStruct=new CObbStruct();
    newObbStruct->deserialize(data);
    CObbStruct::addObbStruct(newObbStruct);
    return(newObbStruct);
}

SIM_DLLEXPORT void geomPlugin_destroyMesh(void* obbStruct)
{
    CObbStruct::removeObbStruct((CObbStruct*)obbStruct);
}

SIM_DLLEXPORT int geomPlugin_getMeshIndicesCount(const void* obbStruct)
{
    const CObbStruct* p=(const CObbStruct*)obbStruct;
    return(int(p->indices.size()));
}

SIM_DLLEXPORT const int* geomPlugin_getMeshIndicesPointer(const void* obbStruct)
{
    CObbStruct* p=(CObbStruct*)obbStruct;
    if (p->indices.size()==0)
        return(nullptr);
    return(&p->indices[0]);
}

SIM_DLLEXPORT const float* geomPlugin_getMeshVerticesPointer(const void* obbStruct)
{
    CObbStruct* p=(CObbStruct*)obbStruct;
    if (p->vertices.size()==0)
        return(nullptr);
    return(&p->vertices[0]);
}

SIM_DLLEXPORT void geomPlugin_getMeshTriangle(const void* obbStruct,float* p1,float* p2,float* p3,int ind)
{
    for (size_t i=0;i<3;i++)
    {
        p1[i]=((const CObbStruct*)obbStruct)->vertices[3*((const CObbStruct*)obbStruct)->indices[3*ind+0]+i];
        p2[i]=((const CObbStruct*)obbStruct)->vertices[3*((const CObbStruct*)obbStruct)->indices[3*ind+1]+i];
        p3[i]=((const CObbStruct*)obbStruct)->vertices[3*((const CObbStruct*)obbStruct)->indices[3*ind+2]+i];
    }
}

SIM_DLLEXPORT char geomPlugin_getMeshMeshCollision(const void* obbStruct1,const void* obbStruct2,const float* shape1M,const float* shape2M,char inverseExploration,float** intersections,int* intersectionsSize,int caching[2])
{
    C4X4Matrix _shape1M;
    _shape1M.copyFromInterface(shape1M);
    C4X4Matrix _shape2M;
    _shape2M.copyFromInterface(shape2M);
    char retVal=0;
    std::vector<float> _intersections;
    std::vector<float>* intersect=nullptr;
    int* cachingTri1=nullptr;
    int* cachingTri2=nullptr;
    if (caching!=nullptr)
    {
        cachingTri1=&caching[0];
        cachingTri2=&caching[1];
    }
    if (intersections!=nullptr)
        intersect=&_intersections;
    if (inverseExploration!=0)
    {
        if ( ((const CObbStruct*)obbStruct2)->obb->checkCollision_obb((const CObbStruct*)obbStruct2,_shape2M,((const CObbStruct*)obbStruct1)->obb,(const CObbStruct*)obbStruct1,_shape1M,intersect,cachingTri2,cachingTri1) )
            retVal=1;
    }
    else
    {
        if ( ((const CObbStruct*)obbStruct1)->obb->checkCollision_obb((const CObbStruct*)obbStruct1,_shape1M,((const CObbStruct*)obbStruct2)->obb,(const CObbStruct*)obbStruct2,_shape2M,intersect,cachingTri1,cachingTri2) )
            retVal=1;
    }

    if (intersections!=nullptr)
    {
        intersections[0]=new float[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_getMeshMeshDistanceIfSmaller(const void* obbStruct1,const void* obbStruct2,const float* shape1M,const float* shape2M,char inverseExploration,float distances[7],int caching[2])
{
    C4X4Matrix _shape1M;
    _shape1M.copyFromInterface(shape1M);
    C4X4Matrix _shape2M;
    _shape2M.copyFromInterface(shape2M);
    float dist=distances[6];
    C3Vector minDistSegPt1,minDistSegPt2;
    int* cachingTri1=nullptr;
    int* cachingTri2=nullptr;
    if (caching!=nullptr)
    {
        cachingTri1=&caching[0];
        cachingTri2=&caching[1];
        caching[0]=-1;
        caching[1]=-1;
    }
    bool res=false;
    if (inverseExploration!=0)
        res=((const CObbStruct*)obbStruct2)->obb->checkDistance_obb((const CObbStruct*)obbStruct2,_shape2M,((const CObbStruct*)obbStruct1)->obb,(const CObbStruct*)obbStruct1,_shape1M,dist,&minDistSegPt2,&minDistSegPt1,cachingTri2,cachingTri1);
    else
        res=((const CObbStruct*)obbStruct1)->obb->checkDistance_obb((const CObbStruct*)obbStruct1,_shape1M,((const CObbStruct*)obbStruct2)->obb,(const CObbStruct*)obbStruct2,_shape2M,dist,&minDistSegPt1,&minDistSegPt2,cachingTri1,cachingTri2);

    if (res)
    {
        distances[0]=minDistSegPt1(0);
        distances[1]=minDistSegPt1(1);
        distances[2]=minDistSegPt1(2);
        distances[3]=minDistSegPt2(0);
        distances[4]=minDistSegPt2(1);
        distances[5]=minDistSegPt2(2);
        distances[6]=dist;
    }
}

SIM_DLLEXPORT char geomPlugin_getMeshPointDistanceIfSmaller(const void* obbStruct,const float* pt,const float* shapeM,float* dist,float* minDistSegPt1,float* minDistSegPt2,int* caching)
{
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    C3Vector _pt(pt);
    C3Vector _minDistSegPt1;
    char retVal=((const CObbStruct*)obbStruct)->obb->checkDistance_pt((const CObbStruct*)obbStruct,_shapeM,_pt,dist[0],&_minDistSegPt1,caching);
    if (retVal!=0)
    {
        _minDistSegPt1.getInternalData(minDistSegPt1);
        minDistSegPt2[0]=pt[0];
        minDistSegPt2[1]=pt[1];
        minDistSegPt2[2]=pt[2];
    }
    return(retVal);
}

SIM_DLLEXPORT int geomPlugin_getMeshVerticesCount(const void* obbStruct)
{
    const CObbStruct* p=(const CObbStruct*)obbStruct;
    return(int(p->vertices.size())/3);
}

SIM_DLLEXPORT char geomPlugin_getRayProxSensorMeshDistanceIfSmaller(const void* obbStruct,const float* shapeM,float* dist,const float* raySegP,float forbiddenDist,const float* raySegL,float cosAngle,float* detectPt,bool fast,bool frontDetection,bool backDetection,char* forbiddenDistTouched,float* triN)
{
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    C3Vector _detectPt,_triN;
    C3Vector _raySegP(raySegP);
    C3Vector _raySegL(raySegL);
    bool* _forbiddenDistTouched=nullptr;
    bool _ff=false;
    if (forbiddenDistTouched!=nullptr)
        _forbiddenDistTouched=&_ff;
    bool retVal=((const CObbStruct*)obbStruct)->obb->checkRaySensorDistance_obb((const CObbStruct*)obbStruct,_shapeM,_raySegP,_raySegL,cosAngle,frontDetection,backDetection,forbiddenDist,fast,dist[0],&_detectPt,&_triN,_forbiddenDistTouched);
    if (retVal)
    {
        _detectPt.copyTo(detectPt);
        _triN.copyTo(triN);
        return(1);
    }
    if (forbiddenDistTouched!=nullptr)
    {
        if (_ff)
            forbiddenDistTouched[0]=1;
        else
            forbiddenDistTouched[0]=0;
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getProxSensorMeshDistanceIfSmaller(const void* obbStruct,const float* shapeM,float* dist,const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,float cosAngle,float* detectPt,bool fast,bool frontDetection,bool backDetection,float* triN)
{
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    C3Vector _detectPt,_triN;
    CVolumePlanes _planesIn(planesIn,planesInSize);
    CVolumePlanes _planesOut(planesOut,planesOutSize);
    char retVal=((const CObbStruct*)obbStruct)->obb->checkSensorDistance_obb((const CObbStruct*)obbStruct,_shapeM,_planesIn,_planesOut,cosAngle,frontDetection,backDetection,fast,dist[0],&_detectPt,&_triN);
    if (retVal!=0)
    {
        _detectPt.copyTo(detectPt);
        _triN.copyTo(triN);
    }
    return(retVal);
}

SIM_DLLEXPORT char geomPlugin_getProxSensorTriangleDistanceIfSmaller(const float* p,const float* v,const float* w,float* dist,const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,float cosAngle,float* detectPt,bool frontDetection,bool backDetection,float* triN)
{
    C3Vector _detectPt,_triN;
    CVolumePlanes _planesIn(planesIn,planesInSize);
    CVolumePlanes _planesOut(planesOut,planesOutSize);
    if (CCalcUtils::getSensorDistance_tri(_planesIn,_planesOut,cosAngle,frontDetection,backDetection,C3Vector(p),C3Vector(v),C3Vector(w),dist[0],&_detectPt,&_triN))
    {
        if (detectPt!=nullptr)
            _detectPt.copyTo(detectPt);
        if (triN!=nullptr)
            _triN.copyTo(triN);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getProxSensorSegmentDistanceIfSmaller(const float* p0,const float* p1,float* dist,const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,float cosAngle,float* detectPt)
{
    C3Vector _p0(p0);
    C3Vector _p1(p1);
    C3Vector _detectPt;
    CVolumePlanes _planesIn(planesIn,planesInSize);
    CVolumePlanes _planesOut(planesOut,planesOutSize);
    if (CCalcUtils::getSensorDistance_segp(_planesIn,_planesOut,cosAngle,_p0,_p1-p0,dist[0],&_detectPt))
    {
        if (detectPt!=nullptr)
            _detectPt.copyTo(detectPt);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT void* geomPlugin_createPointCloud(const float* points,int pointCnt,float cellS,int maxPointCnt,const unsigned char* rgbData,float proximityTol)
{
    CPcStruct* pcStruct=new CPcStruct(cellS,maxPointCnt,points,pointCnt,rgbData,false,proximityTol);
    return(pcStruct);
}

SIM_DLLEXPORT void* geomPlugin_createColorPointCloud(const float* points,int pointCnt,float cellS,int maxPointCnt,const unsigned char* rgbData,float proximityTol)
{
    CPcStruct* pcStruct=new CPcStruct(cellS,maxPointCnt,points,pointCnt,rgbData,true,proximityTol);
    return(pcStruct);
}

SIM_DLLEXPORT void* geomPlugin_copyPointCloud(const void* pcStruct)
{
    CPcStruct* newPcStruct=((const CPcStruct*)pcStruct)->copyYourself();
    return(newPcStruct);
}

SIM_DLLEXPORT void geomPlugin_scalePointCloud(void* pcStruct,float f)
{
    ((CPcStruct*)pcStruct)->scaleYourself(f);
}

SIM_DLLEXPORT unsigned char* geomPlugin_getPointCloudSerializationData(const void* pcStruct,int* dataSize)
{
    return(((const CPcStruct*)pcStruct)->serialize(dataSize[0]));
}

SIM_DLLEXPORT void* geomPlugin_getPointCloudFromSerializationData(const unsigned char* data)
{
    CPcStruct* pcStruct=new CPcStruct();
    pcStruct->deserialize(data);
    return(pcStruct);
}

SIM_DLLEXPORT void geomPlugin_destroyPointCloud(void* pcStruct)
{
    delete ((CPcStruct*)pcStruct);
}

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPointCloud(void* pcStruct,const float* points,int pointCnt,const unsigned char* rgbData,float proximityTol)
{
    ((CPcStruct*)pcStruct)->add_pts(points,pointCnt,rgbData,false,proximityTol);
}

SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPointCloud(void* pcStruct,const float* points,int pointCnt,const unsigned char* rgbData,float proximityTol)
{
    ((CPcStruct*)pcStruct)->add_pts(points,pointCnt,rgbData,true,proximityTol);
}

SIM_DLLEXPORT char geomPlugin_removePointsFromPointCloud(void* pcStruct,const float* points,int pointCnt,float proximityTol,int* count)
{
    if (((CPcStruct*)pcStruct)->delete_pts(points,pointCnt,proximityTol,count))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_removeOctreeFromPointCloud(void* pcStruct,const float* ptcloudM,const void* ocStruct,const float* octreeM,int* count)
{
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    if (((CPcStruct*)pcStruct)->delete_octree(_ptcloudM,(const COcStruct*)ocStruct,_octreeM,count))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_intersectPointsWithPointCloud(void* pcStruct,const float* points,int pointCnt,float proximityTol)
{
    if (((CPcStruct*)pcStruct)->intersect_pts(points,pointCnt,proximityTol))
        return(1);
    return(0);
}

SIM_DLLEXPORT float* geomPlugin_getOctreeCornersFromPointCloud(const void* pcStruct,int* cubeCnt)
{
    std::vector<float> pts;
    ((const CPcStruct*)pcStruct)->getOctreeCorners(pts);
    float* buff=new float[pts.size()];
    for (size_t i=0;i<pts.size();i++)
        buff[i]=pts[i];
    cubeCnt[0]=int(pts.size())/24;
    return(buff);
}

SIM_DLLEXPORT float* geomPlugin_getPointDataFromPointCloud(const void* pcStruct,int* pointCnt)
{
    std::vector<float> dat;
    ((const CPcStruct*)pcStruct)->getPointsPosAndRgb_all(dat);
    float* retVal=new float[dat.size()];
    for (size_t i=0;i<dat.size();i++)
        retVal[i]=dat[i];
    pointCnt[0]=int(dat.size())/6;
    return(retVal);
}

SIM_DLLEXPORT float* geomPlugin_getPartialPointDataFromPointCloud(const void* pcStruct,int* pointCnt,float prop)
{
    std::vector<float> dat;
    ((const CPcStruct*)pcStruct)->getPointsPosAndRgb_subset(dat,prop);
    float* retVal=new float[dat.size()];
    for (size_t i=0;i<dat.size();i++)
        retVal[i]=dat[i];
    pointCnt[0]=int(dat.size())/6;
    return(retVal);
}

SIM_DLLEXPORT float* geomPlugin_getPointsFromPointCloudCache(const void* pcStruct,const float* ptcloudM,const long long int pcCaching,int* pointCnt,float* transfM)
{
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    C4X4Matrix _transf;
    size_t _pointCnt;
    const float* points=((const CPcStruct*)pcStruct)->getPoints(_ptcloudM,pcCaching,&_pointCnt,_transf);
    if (points!=nullptr)
    {
        pointCnt[0]=(int)_pointCnt;
        _transf.copyToInterface(transfM);
    }
    return((float*)points);
}

SIM_DLLEXPORT int geomPlugin_getPointCloudNonEmptyCellCount(const void* pcStruct)
{
    return(int(((const CPcStruct*)pcStruct)->countCellsWithContent()));
}

SIM_DLLEXPORT char geomPlugin_getPointCloudPointDistanceIfSmaller(const void* pcStruct,const float* ptcloudM,const float* point,float* ray,long long int* pcCaching)
{
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    C3Vector _point(point);
    C3Vector pcMinDistSegAbsPt;
    C3Vector ptMinDistSegAbsPt;
    float dist=ray[6];
    if (((const CPcStruct*)pcStruct)->getDistance_pt(_ptcloudM,_point,dist,&pcMinDistSegAbsPt,&ptMinDistSegAbsPt,(unsigned long long int*)pcCaching))
    {
        pcMinDistSegAbsPt.copyTo(ray+0);
        ptMinDistSegAbsPt.copyTo(ray+3);
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getPointCloudMeshDistanceIfSmaller(const void* pcStruct,const void* obbStruct,const float* ptcloudM,const float* shapeM,float* ray,long long int* pcCaching,int* obbCaching)
{
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    C3Vector pcMinDistSegAbsPt;
    C3Vector shapeMinDistSegAbsPt;
    float dist=ray[6];
    if (((const CPcStruct*)pcStruct)->getDistance_shape((const CObbStruct*)obbStruct,_ptcloudM,_shapeM,dist,&pcMinDistSegAbsPt,&shapeMinDistSegAbsPt,(unsigned long long int*)pcCaching,obbCaching))
    {
        pcMinDistSegAbsPt.copyTo(ray+0);
        shapeMinDistSegAbsPt.copyTo(ray+3);
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getPointCloudPointCloudDistanceIfSmaller(const void* pc1Struct,const void* pc2Struct,const float* ptcloud1M,const float* ptcloud2M,float* ray,long long int* pc1Caching,long long int* pc2Caching)
{
    C4X4Matrix _ptcloud1M;
    _ptcloud1M.copyFromInterface(ptcloud1M);
    C4X4Matrix _ptcloud2M;
    _ptcloud2M.copyFromInterface(ptcloud2M);
    C3Vector pc1MinDistSegAbsPt;
    C3Vector pc2MinDistSegAbsPt;
    float dist=ray[6];
    if (((const CPcStruct*)pc1Struct)->getDistance_ptcloud((const CPcStruct*)pc2Struct,_ptcloud1M,_ptcloud2M,dist,&pc1MinDistSegAbsPt,&pc2MinDistSegAbsPt,(unsigned long long int*)pc1Caching,(unsigned long long int*)pc2Caching))
    {
        pc1MinDistSegAbsPt.copyTo(ray+0);
        pc2MinDistSegAbsPt.copyTo(ray+3);
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getProxSensorPointCloudDistanceIfSmaller(const void* pcStruct,const float* ptcloudM,float* dist,const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,float* detectPt,char fast)
{
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    C3Vector _detectPt(detectPt);
    CVolumePlanes _planesIn(planesIn,planesInSize);
    CVolumePlanes _planesOut(planesOut,planesOutSize);
    if (((const CPcStruct*)pcStruct)->getSensorDistance(_ptcloudM,_planesIn,_planesOut,fast!=0,dist[0],&_detectPt))
    {
        _detectPt.copyTo(detectPt);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const float* points,int pointCnt,float cellS,const unsigned char* rgbData,unsigned int usrData)
{
    COcStruct* ocStruct=new COcStruct(cellS,points,(int)pointCnt,rgbData,&usrData,false);
    return(ocStruct);
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const float* points,int pointCnt,float cellS,const unsigned char* rgbData,const unsigned int* usrData)
{
    COcStruct* ocStruct=new COcStruct(cellS,points,(int)pointCnt,rgbData,usrData,true);
    return(ocStruct);
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const float* octreeM,const void* obbStruct,const float* shapeM,float cellS,const unsigned char* rgbData,unsigned int usrData)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    COcStruct* ocStruct=new COcStruct(_octreeM,cellS,(const CObbStruct*)obbStruct,_shapeM,rgbData,usrData);
    return(ocStruct);
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const float* octree1M,const void* oc2Struct,const float* octree2M,float cell1S,const unsigned char* rgbData,unsigned int usrData)
{
    C4X4Matrix _octree1M;
    _octree1M.copyFromInterface(octree1M);
    C4X4Matrix _octree2M;
    _octree2M.copyFromInterface(octree2M);
    COcStruct* oc1Struct=new COcStruct(_octree1M,cell1S,(const COcStruct*)oc2Struct,_octree2M,rgbData,usrData);
    return(oc1Struct);
}

SIM_DLLEXPORT void* geomPlugin_copyOctree(const void* ocStruct)
{
    COcStruct* newOcStruct=((const COcStruct*)ocStruct)->copyYourself();
    return(newOcStruct);
}

SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,float f)
{
    ((COcStruct*)ocStruct)->scaleYourself(f);
}

SIM_DLLEXPORT unsigned char* geomPlugin_getOctreeSerializationData(const void* ocStruct,int* dataSize)
{
    return(((const COcStruct*)ocStruct)->serialize(dataSize[0]));
}

SIM_DLLEXPORT void* geomPlugin_getOctreeFromSerializationData(const unsigned char* data)
{
    COcStruct* octreeInfo=new COcStruct();
    octreeInfo->deserialize(data);
    return(octreeInfo);
}

SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct)
{
    delete ((COcStruct*)ocStruct);
}

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const float* points,int pointCnt,const unsigned char* rgbData,unsigned int usrData)
{
    ((COcStruct*)ocStruct)->add_pts(points,pointCnt,rgbData,&usrData,false);
}

SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const float* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData)
{
    ((COcStruct*)ocStruct)->add_pts(points,pointCnt,rgbData,usrData,true);
}

SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const float* octreeM,const void* obbStruct,const float* shapeM,const unsigned char* rgbData,unsigned int usrData)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    ((COcStruct*)ocStruct)->add_shape(_octreeM,(const CObbStruct*)obbStruct,_shapeM,rgbData,usrData);
}

SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const float* octree1M,const void* oc2Struct,const float* octree2M,const unsigned char* rgbData,unsigned int usrData)
{
    C4X4Matrix _octree1M;
    _octree1M.copyFromInterface(octree1M);
    C4X4Matrix _octree2M;
    _octree2M.copyFromInterface(octree2M);
    ((COcStruct*)oc1Struct)->add_octree(_octree1M,(const COcStruct*)oc2Struct,_octree2M,rgbData,usrData);
}

SIM_DLLEXPORT char geomPlugin_removePointsFromOctree(void* ocStruct,const float* points,int pointCnt)
{
    if (((COcStruct*)ocStruct)->deleteVoxels_pts(points,pointCnt))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_removeMeshFromOctree(void* ocStruct,const float* octreeM,const void* obbStruct,const float* shapeM)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    if (((COcStruct*)ocStruct)->deleteVoxels_shape(_octreeM,(const CObbStruct*)obbStruct,_shapeM))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_removeOctreeFromOctree(void* oc1Struct,const float* octree1M,const void* oc2Struct,const float* octree2M)
{
    C4X4Matrix _octree1M;
    _octree1M.copyFromInterface(octree1M);
    C4X4Matrix _octree2M;
    _octree2M.copyFromInterface(octree2M);
    if (((COcStruct*)oc1Struct)->deleteVoxels_octree(_octree1M,(const COcStruct*)oc2Struct,_octree2M))
        return(1);
    return(0);
}

SIM_DLLEXPORT float* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* cubeCnt)
{
    std::vector<float> pts;
    ((const COcStruct*)ocStruct)->getOctreeCorners(pts);
    float* buff=new float[pts.size()];
    for (size_t i=0;i<pts.size();i++)
        buff[i]=pts[i];
    cubeCnt[0]=int(pts.size())/24;
    return(buff);
}

SIM_DLLEXPORT float* geomPlugin_getOctreeVoxels(const void* ocStruct,int* voxelCnt)
{
    std::vector<float> posAndRgb;
    ((const COcStruct*)ocStruct)->getVoxelsPosAndRgb(posAndRgb,nullptr);
    float* buff=new float[posAndRgb.size()];
    for (size_t i=0;i<posAndRgb.size();i++)
        buff[i]=posAndRgb[i];
    voxelCnt[0]=int(posAndRgb.size())/6;
    return(buff);
}

SIM_DLLEXPORT char geomPlugin_getOctreeCellFromCache(const void* ocStruct,const float* octreeM,const long long int ocCaching,float* cellS,float* cellM)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _cellM;
    bool retVal=((const COcStruct*)ocStruct)->getCell(_octreeM,(const unsigned long long int)ocCaching,cellS[0],_cellM);
    if (retVal)
        _cellM.copyToInterface(cellM);
    return(retVal);
}

SIM_DLLEXPORT char geomPlugin_getOctreePointCollision(const void* ocStruct,const float* octreeM,const float* point,unsigned int* usrData,unsigned long long int* ocCaching)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    // point rel to the octree frame:
    C4X4Matrix m(_octreeM);
    m.X+=_octreeM.M*((const COcStruct*)ocStruct)->getBoxPos();
    C3Vector _point(m.getInverse()*C3Vector(point));

    if (((const COcStruct*)ocStruct)->doCollide_pt(_point,usrData,ocCaching))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreePointsCollision(const void* ocStruct,const float* octreeM,const float* points,int pointCnt)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    // points rel to the octree frame:
    C4X4Matrix m(_octreeM);
    m.X+=_octreeM.M*((const COcStruct*)ocStruct)->getBoxPos();
    C4X4Matrix mi(m.getInverse());
    std::vector<float> _points;
    for (int i=0;i<pointCnt;i++)
    {
        C3Vector pt(&points[3*i]);
        pt*=mi;
        _points.push_back(pt(0));
        _points.push_back(pt(1));
        _points.push_back(pt(2));
    }
    if (((const COcStruct*)ocStruct)->doCollide_pts(_points))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreeMeshCollision(const void* ocStruct,const float* octreeM,const void* obbStruct,const float* shapeM)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    if (((const COcStruct*)ocStruct)->doCollide_shape(_octreeM,(const CObbStruct*)obbStruct,_shapeM,nullptr,nullptr))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreePointCloudCollision(const void* ocStruct,const float* octreeM,const void* pcStruct,const float* ptcloudM)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    if (((const COcStruct*)ocStruct)->doCollide_ptcloud(_octreeM,(const CPcStruct*)pcStruct,_ptcloudM,nullptr,nullptr))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const float* octree1M,const void* oc2Struct,const float* octree2M)
{
    C4X4Matrix _octree1M;
    _octree1M.copyFromInterface(octree1M);
    C4X4Matrix _octree2M;
    _octree2M.copyFromInterface(octree2M);
    if (((const COcStruct*)oc1Struct)->doCollide_octree(_octree1M,(const COcStruct*)oc2Struct,_octree2M,nullptr,nullptr))
        return(1);
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const float* octreeM,const float* point,float* ray,long long int* ocCaching)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    // pt is relative to octree frame:
    C4X4Matrix m(_octreeM);
    m.X+=_octreeM.M*((const COcStruct*)ocStruct)->getBoxPos();
    C3Vector _point(m.getInverse()*C3Vector(point));
    C3Vector minDistSegPt1;
    float dist=ray[6];
    if (((const COcStruct*)ocStruct)->getDistance_pt(_point,dist,&minDistSegPt1,(unsigned long long int*)ocCaching))
    {
        minDistSegPt1*=m;
        minDistSegPt1.copyTo(ray);
        ray[3]=point[0];
        ray[4]=point[1];
        ray[5]=point[2];
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreeMeshDistanceIfSmaller(const void* ocStruct,const void* obbStruct,const float* octreeM,const float* shapeM,float* ray,long long int* ocCaching,int* obbCaching)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _shapeM;
    _shapeM.copyFromInterface(shapeM);
    float dist=ray[6];
    C3Vector ocMinDistSegPt,shapeMinDistSegPt;
    if (((const COcStruct*)ocStruct)->getDistance_shape(_octreeM,(const CObbStruct*)obbStruct,_shapeM,dist,&ocMinDistSegPt,&shapeMinDistSegPt,(unsigned long long int*)ocCaching,obbCaching))
    {
        ocMinDistSegPt.copyTo(ray);
        shapeMinDistSegPt.copyTo(ray+3);
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreePointCloudDistanceIfSmaller(const void* ocStruct,const void* pcStruct,const float* octreeM,const float* ptcloudM,float* ray,long long int* ocCaching,long long int* pcCaching)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C4X4Matrix _ptcloudM;
    _ptcloudM.copyFromInterface(ptcloudM);
    float dist=ray[6];
    C3Vector ocMinDistSegPt,pcMinDistSegPt;
    if (((const COcStruct*)ocStruct)->getDistance_ptcloud(_octreeM,(const CPcStruct*)pcStruct,_ptcloudM,dist,&ocMinDistSegPt,&pcMinDistSegPt,(unsigned long long int*)ocCaching,(unsigned long long int*)pcCaching))
    {
        ocMinDistSegPt.copyTo(ray);
        pcMinDistSegPt.copyTo(ray+3);
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const void* oc2Struct,const float* octree1M,const float* octree2M,float* ray,long long int* oc1Caching,long long int* oc2Caching,char isCoherent)
{
    C4X4Matrix _octree1M;
    _octree1M.copyFromInterface(octree1M);
    C4X4Matrix _octree2M;
    _octree2M.copyFromInterface(octree2M);
    float dist=ray[6];
    C3Vector oc1MinDistSegPt,oc2MinDistSegPt;
    if (((const COcStruct*)oc1Struct)->getDistance_octree(_octree1M,(const COcStruct*)oc2Struct,_octree2M,dist,&oc1MinDistSegPt,&oc2MinDistSegPt,(unsigned long long int*)oc1Caching,(unsigned long long int*)oc2Caching))
    {
        oc1MinDistSegPt.copyTo(ray);
        oc2MinDistSegPt.copyTo(ray+3);
        ray[6]=dist;
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getRayProxSensorOctreeDistanceIfSmaller(const void* ocStruct,const float* octreeM,float* dist,const float* raySegP,const float* raySegL,float cosAngle,float* detectPt,char fast,char frontDetection,char backDetection,float* triN)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    C3Vector _raySegP(raySegP);
    C3Vector _raySegL(raySegL);
    C3Vector _detectPt(detectPt);
    C3Vector _triN(triN);
    bool retVal=((const COcStruct*)ocStruct)->getRaySensorDistance(_octreeM,_raySegP,_raySegL,0.0f,cosAngle,frontDetection!=0,backDetection!=0,fast!=0,dist[0],&_detectPt,&_triN,nullptr);
    if (retVal)
    {
        _detectPt.copyTo(detectPt);
        _triN.copyTo(triN);
        return(1);
    }
    return(0);
}

SIM_DLLEXPORT char geomPlugin_getProxSensorOctreeDistanceIfSmaller(const void* ocStruct,const float* octreeM,float* dist,const float* planesIn,int planesInSize,const float* planesOut,int planesOutSize,float cosAngle,float* detectPt,char fast,char frontDetection,char backDetection,float* triN)
{
    C4X4Matrix _octreeM;
    _octreeM.copyFromInterface(octreeM);
    CVolumePlanes _planesIn(planesIn,planesInSize);
    CVolumePlanes _planesOut(planesOut,planesOutSize);
    C3Vector _detectPt(detectPt);
    C3Vector _triN(triN);
    if (((const COcStruct*)ocStruct)->getSensorDistance(_octreeM,_planesIn,_planesOut,cosAngle,frontDetection!=0,backDetection!=0,fast!=0,dist[0],&_detectPt,&_triN))
    {
        _detectPt.copyTo(detectPt);
        _triN.copyTo(triN);
        return(1);
    }
    return(0);
}
*/
