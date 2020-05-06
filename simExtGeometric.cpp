#include "simExtGeometric.h"
#include "simLib.h"
#include <iostream>
#include <cstdio>
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
#ifdef SIM_MATH_DOUBLE
#define realData doubleData
#define sim_script_arg_real sim_script_arg_double
#else
#define realData floatData
#define sim_script_arg_real sim_script_arg_float
#endif

static LIBRARY simLib;

static std::map<int,CObbStruct*> _meshData;
static int _nextMeshDataHandle=0;
static std::map<int,COcStruct*> _octreeData;
static int _nextOctreeDataHandle=0;
static std::map<int,CPcStruct*> _ptcloudData;
static int _nextPtcloudDataHandle=0;

bool canOutputMsg(int msgType)
{
    int plugin_verbosity = sim_verbosity_default;
    simGetModuleInfo("Geometric",sim_moduleinfo_verbosity,nullptr,&plugin_verbosity);
    return(plugin_verbosity>=msgType);
}

void outputMsg(int msgType,const char* msg)
{
    if (canOutputMsg(msgType))
        printf("%s\n",msg);
}

// --------------------------------------------------------------------------------------
// simGeom.getMeshMeshCollision
// --------------------------------------------------------------------------------------
#define LUA_GETMESHMESHCOLLISION_COMMAND_PLUGIN "simGeom.getMeshMeshCollision@Geometric"
#define LUA_GETMESHMESHCOLLISION_COMMAND "simGeom.getMeshMeshCollision"

const int inArgs_GETMESHMESHCOLLISION[]={
    8,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
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
            C3Vector mesh1Pos(&(inData->at(1).realData[0]));
            C4Vector mesh1Q(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector mesh2Pos(&(inData->at(4).realData[0]));
            C4Vector mesh2Q(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
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
            std::vector<simReal>* _intersect=nullptr;
            std::vector<simReal> intersect;
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector octreePos(&(inData->at(4).realData[0]));
            C4Vector octreeQ(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector triPt1(&inData->at(3).realData[0]);
            C3Vector triPt2(&inData->at(4).realData[0]);
            C3Vector triPt3(&inData->at(5).realData[0]);
            int cache=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()==1) )
                cache=inData->at(6).int32Data[0];
            bool returnIntersections=false;
            if ( (inData->size()>=8)&&(inData->at(7).boolData.size()==1) )
                returnIntersections=inData->at(7).boolData[0];
            std::vector<simReal>* _intersect=nullptr;
            std::vector<simReal> intersect;
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector segPt1(&inData->at(3).realData[0]);
            C3Vector segPt2(&inData->at(4).realData[0]);
            int cache=-1;
            if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
                cache=inData->at(5).int32Data[0];
            bool returnIntersections=false;
            if ( (inData->size()>=7)&&(inData->at(6).boolData.size()==1) )
                returnIntersections=inData->at(6).boolData[0];
            std::vector<simReal>* _intersect=nullptr;
            std::vector<simReal> intersect;
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
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
            C3Vector octree1Pos(&(inData->at(1).realData[0]));
            C4Vector octree1Q(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector octree2Pos(&(inData->at(4).realData[0]));
            C4Vector octree2Q(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector ptcloudPos(&(inData->at(4).realData[0]));
            C4Vector ptcloudQ(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector triPt1(&(inData->at(3).realData[0]));
            C3Vector triPt2(&(inData->at(4).realData[0]));
            C3Vector triPt3(&(inData->at(5).realData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(6).doubleData[0]);
            bool coll=geom_getOctreeTriangleCollision(it->second,C7Vector(octreeQ,octreePos),triPt1,triPt2-triPt1,triPt3-triPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector segPt1(&(inData->at(3).realData[0]));
            C3Vector segPt2(&(inData->at(4).realData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(5).doubleData[0]);
            bool coll=geom_getOctreeSegmentCollision(it->second,C7Vector(octreeQ,octreePos),segPt1,segPt2-segPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector point(&(inData->at(3).realData[0]));
            unsigned long long int cache=0;
            if ( (inData->size()>=5)&&(inData->at(4).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(4).doubleData[0]);
            bool coll=geom_getOctreePointCollision(it->second,C7Vector(octreeQ,octreePos),point,nullptr,&cache);
            D.pushOutData(CScriptFunctionDataItem(coll));
            if (coll)
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector mesh1Pos(&(inData->at(1).realData[0]));
            C4Vector mesh1Q(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector mesh2Pos(&(inData->at(4).realData[0]));
            C4Vector mesh2Q(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
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
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector octreePos(&(inData->at(4).realData[0]));
            C4Vector octreeQ(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
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
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector ptcloudPos(&(inData->at(4).realData[0]));
            C4Vector ptcloudQ(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
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
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector octree1Pos(&(inData->at(1).realData[0]));
            C4Vector octree1Q(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector octree2Pos(&(inData->at(4).realData[0]));
            C4Vector octree2Q(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
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
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector ptcloudPos(&(inData->at(4).realData[0]));
            C4Vector ptcloudQ(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
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
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                std::vector<double> c;
                c.push_back(double(cache1));
                c.push_back(double(cache2));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector triPt1(&(inData->at(3).realData[0]));
            C3Vector triPt2(&(inData->at(4).realData[0]));
            C3Vector triPt3(&(inData->at(5).realData[0]));
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(7).doubleData[0]);
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeTriangleDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector segPt1(&(inData->at(3).realData[0]));
            C3Vector segPt2(&(inData->at(4).realData[0]));
            simReal dist=REAL_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).realData.size()==1) )
                dist=inData->at(5).realData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(6).doubleData[0]);
            C3Vector distPt1,distPt2;
            bool smaller=geom_getOctreeSegmentDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp2));
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector octreePos(&(inData->at(1).realData[0]));
            C4Vector octreeQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector point(&(inData->at(3).realData[0]));
            simReal dist=REAL_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).realData.size()==1) )
                dist=inData->at(4).realData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=static_cast<unsigned long long int>(inData->at(5).doubleData[0]);
            C3Vector distPt;
            bool smaller=geom_getOctreePointDistanceIfSmaller(it->second,C7Vector(octreeQ,octreePos),point,dist,&distPt,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt.data,distPt.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                D.pushOutData(CScriptFunctionDataItem(double(cache)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector triPt1(&inData->at(3).realData[0]);
            C3Vector triPt2(&inData->at(4).realData[0]);
            C3Vector triPt3(&inData->at(5).realData[0]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
            int cache=-1;
            if ( (inData->size()>=8)&&(inData->at(7).int32Data.size()==1) )
                cache=inData->at(7).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshTriangleDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector segPt1(&inData->at(3).realData[0]);
            C3Vector segPt2(&inData->at(4).realData[0]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).realData.size()==1) )
                dist=inData->at(5).realData[0];
            int cache=-1;
            if ( (inData->size()>=7)&&(inData->at(6).int32Data.size()==1) )
                cache=inData->at(6).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshSegmentDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector pt1(&inData->at(3).realData[0]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).realData.size()==1) )
                dist=inData->at(4).realData[0];
            int cache=-1;
            if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
                cache=inData->at(5).int32Data[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getMeshPointDistanceIfSmaller(it->second,C7Vector(meshQ,meshPos),pt1,dist,&distPt1,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_bool,0,
};

void LUA_GETBOXBOXDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXBOXDISTANCE,inArgs_GETBOXBOXDISTANCE[0]-1,LUA_GETBOXBOXDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector box1Pos(&(inData->at(0).realData[0]));
        C4Vector box1Q(inData->at(1).realData[3],inData->at(1).realData[0],inData->at(1).realData[1],inData->at(1).realData[2]);
        C3Vector box1Hs(&(inData->at(2).realData[0]));
        C3Vector box2Pos(&(inData->at(3).realData[0]));
        C4Vector box2Q(inData->at(4).realData[3],inData->at(4).realData[0],inData->at(4).realData[1],inData->at(4).realData[2]);
        C3Vector box2Hs(&(inData->at(5).realData[0]));
        bool boxesAreSolid=inData->at(6).boolData[0];
        bool altRoutine=false;
        if ( (inData->size()>7)&&(inData->at(7).boolData.size()==1) )
            altRoutine=inData->at(7).boolData[0];
        C3Vector distPt1,distPt2;
        simReal dist=geom_getBoxBoxDistance(C7Vector(box1Q,box1Pos),box1Hs,C7Vector(box2Q,box2Pos),box2Hs,boxesAreSolid,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp1));
        std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
};

void LUA_GETBOXTRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXTRIANGLEDISTANCE,inArgs_GETBOXTRIANGLEDISTANCE[0]-1,LUA_GETBOXTRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).realData[0]));
        C4Vector boxQ(inData->at(1).realData[3],inData->at(1).realData[0],inData->at(1).realData[1],inData->at(1).realData[2]);
        C3Vector boxHs(&(inData->at(2).realData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector p1(&(inData->at(4).realData[0]));
        C3Vector p2(&(inData->at(5).realData[0]));
        C3Vector p3(&(inData->at(6).realData[0]));
        bool altRoutine=false;
        if ( (inData->size()>7)&&(inData->at(7).boolData.size()==1) )
            altRoutine=inData->at(7).boolData[0];
        C3Vector distPt1,distPt2;
        simReal dist=geom_getBoxTriangleDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,p1,p2-p1,p3-p1,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(pp1));
        std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
};

void LUA_GETBOXSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXSEGMENTDISTANCE,inArgs_GETBOXSEGMENTDISTANCE[0]-1,LUA_GETBOXSEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).realData[0]));
        C4Vector boxQ(inData->at(1).realData[3],inData->at(1).realData[0],inData->at(1).realData[1],inData->at(1).realData[2]);
        C3Vector boxHs(&(inData->at(2).realData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector segP1(&(inData->at(4).realData[0]));
        C3Vector segP2(&(inData->at(5).realData[0]));
        bool altRoutine=false;
        if ( (inData->size()>6)&&(inData->at(6).boolData.size()==1) )
            altRoutine=inData->at(6).boolData[0];
        C3Vector distPt1,distPt2;
        simReal dist=geom_getBoxSegmentDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,segP1,segP2-segP1,&distPt1,&distPt2,altRoutine);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<simReal> p2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_bool,0,
    sim_script_arg_real|sim_lua_arg_table,3,
};

void LUA_GETBOXPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETBOXPOINTDISTANCE,inArgs_GETBOXPOINTDISTANCE[0],LUA_GETBOXPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector boxPos(&(inData->at(0).realData[0]));
        C4Vector boxQ(inData->at(1).realData[3],inData->at(1).realData[0],inData->at(1).realData[1],inData->at(1).realData[2]);
        C3Vector boxHs(&(inData->at(2).realData[0]));
        bool boxIsSolid=inData->at(3).boolData[0];
        C3Vector point(&(inData->at(4).realData[0]));
        C3Vector distPt;
        simReal dist=geom_getBoxPointDistance(C7Vector(boxQ,boxPos),boxHs,boxIsSolid,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt.data,distPt.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLETRIANGLEDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLETRIANGLEDISTANCE,inArgs_GETTRIANGLETRIANGLEDISTANCE[0],LUA_GETTRIANGLETRIANGLEDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tpa1(&(inData->at(0).realData[0]));
        C3Vector tpa2(&(inData->at(1).realData[0]));
        C3Vector tpa3(&(inData->at(2).realData[0]));
        C3Vector tpb1(&(inData->at(3).realData[0]));
        C3Vector tpb2(&(inData->at(4).realData[0]));
        C3Vector tpb3(&(inData->at(5).realData[0]));
        C3Vector distPt1,distPt2;
        simReal dist=geom_getTriangleTriangleDistance(tpa1,tpa2-tpa1,tpa3-tpa1,tpb1,tpb2-tpb1,tpb3-tpb1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<simReal> p2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLESEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLESEGMENTDISTANCE,inArgs_GETTRIANGLESEGMENTDISTANCE[0],LUA_GETTRIANGLESEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tp1(&(inData->at(0).realData[0]));
        C3Vector tp2(&(inData->at(1).realData[0]));
        C3Vector tp3(&(inData->at(2).realData[0]));
        C3Vector segP1(&(inData->at(3).realData[0]));
        C3Vector segP2(&(inData->at(4).realData[0]));
        C3Vector distPt1,distPt2;
        simReal dist=geom_getTriangleSegmentDistance(tp1,tp2-tp1,tp3-tp1,segP1,segP2-segP1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<simReal> p2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
};

void LUA_GETTRIANGLEPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRIANGLEPOINTDISTANCE,inArgs_GETTRIANGLEPOINTDISTANCE[0],LUA_GETTRIANGLEPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector tp1(&(inData->at(0).realData[0]));
        C3Vector tp2(&(inData->at(1).realData[0]));
        C3Vector tp3(&(inData->at(2).realData[0]));
        C3Vector point(&(inData->at(3).realData[0]));
        C3Vector distPt;
        simReal dist=geom_getTrianglePointDistance(tp1,tp2-tp1,tp3-tp1,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt.data,distPt.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
};

void LUA_GETSEGMENTSEGMENTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSEGMENTSEGMENTDISTANCE,inArgs_GETSEGMENTSEGMENTDISTANCE[0],LUA_GETSEGMENTSEGMENTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector segaP1(&(inData->at(0).realData[0]));
        C3Vector segaP2(&(inData->at(1).realData[0]));
        C3Vector segbP1(&(inData->at(2).realData[0]));
        C3Vector segbP2(&(inData->at(3).realData[0]));
        C3Vector distPt1,distPt2;
        simReal dist=geom_getSegmentSegmentDistance(segaP1,segaP2-segaP1,segbP1,segbP2-segbP1,&distPt1,&distPt2);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt1.data,distPt1.data+3);
        D.pushOutData(CScriptFunctionDataItem(p1));
        std::vector<simReal> p2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
};

void LUA_GETSEGMENTPOINTDISTANCE_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETSEGMENTPOINTDISTANCE,inArgs_GETSEGMENTPOINTDISTANCE[0],LUA_GETSEGMENTPOINTDISTANCE_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector segPt1(&(inData->at(0).realData[0]));
        C3Vector segPt2(&(inData->at(1).realData[0]));
        C3Vector point(&(inData->at(3).realData[0]));
        C3Vector distPt;
        simReal dist=geom_getSegmentPointDistance(segPt1,segPt2-segPt1,point,&distPt);
        D.pushOutData(CScriptFunctionDataItem(dist));
        std::vector<simReal> p1(distPt.data,distPt.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,0,
    sim_script_arg_int32|sim_lua_arg_table,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0,
    sim_script_arg_int32,0,
};

void LUA_CREATEMESH_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEMESH,inArgs_CREATEMESH[0]-4,LUA_CREATEMESH_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        simReal maxTriS=simReal(0.3);
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=3)&&(inData->at(2).realData.size()>=3) )
            pos.set(&inData->at(2).realData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=4)&&(inData->at(3).realData.size()>=4) )
        {
            quat(0)=inData->at(3).realData[3];
            quat(1)=inData->at(3).realData[0];
            quat(2)=inData->at(3).realData[1];
            quat(3)=inData->at(3).realData[2];
        }
        C7Vector tr(quat,pos);
        if ( (inData->size()>=5)&&(inData->at(4).realData.size()==1) )
            maxTriS=inData->at(4).realData[0];
        int maxTri=8;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()==1) )
            maxTri=inData->at(5).int32Data[0];
        CObbStruct* obbStruct=geom_createMesh(&inData->at(0).realData[0],int(inData->at(0).realData.size()),&inData->at(1).int32Data[0],int(inData->at(1).int32Data.size()),&tr,maxTriS,maxTri);
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
    sim_script_arg_real,0,
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
            geom_scaleMesh(it->second,inData->at(1).realData[0]);
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
    sim_script_arg_real|sim_lua_arg_table,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0,
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
        if ( (inData->size()>=2)&&(inData->at(1).realData.size()>=3) )
            pos.set(&inData->at(1).realData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).realData.size()>=4) )
        {
            quat(0)=inData->at(2).realData[3];
            quat(1)=inData->at(2).realData[0];
            quat(2)=inData->at(2).realData[1];
            quat(3)=inData->at(2).realData[2];
        }
        C7Vector tr(quat,pos);
        simReal cellS=simReal(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).realData.size()==1) )
            cellS=inData->at(3).realData[0];
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

        COcStruct* ocStruct=geom_createOctreeFromPoints(&inData->at(0).realData[0],inData->at(0).realData.size()/3,&tr,cellS,rgb,(unsigned int)usrdata);
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
    sim_script_arg_real|sim_lua_arg_table,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0,
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
        if ( (inData->size()>=2)&&(inData->at(1).realData.size()>=3) )
            pos.set(&inData->at(1).realData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).realData.size()>=4) )
        {
            quat(0)=inData->at(2).realData[3];
            quat(1)=inData->at(2).realData[0];
            quat(2)=inData->at(2).realData[1];
            quat(3)=inData->at(2).realData[2];
        }
        C7Vector tr(quat,pos);
        simReal cellS=simReal(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).realData.size()==1) )
            cellS=inData->at(3).realData[0];
        unsigned char* rgb=nullptr;
        std::vector<unsigned char> _rgb;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()>=inData->at(0).realData.size()) )
        {
            _rgb.resize(inData->at(4).int32Data.size());
            for (size_t i=0;i<inData->at(4).int32Data.size();i++)
                _rgb[i]=(unsigned char)inData->at(4).int32Data[i];
            rgb=&_rgb[0];
        }
        unsigned int* usrdata=nullptr;
        std::vector<unsigned int> _usrdata;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=inData->at(0).realData.size()/3) )
        {
            _usrdata.resize(inData->at(5).int32Data.size());
            for (size_t i=0;i<inData->at(5).int32Data.size();i++)
                _usrdata[i]=(unsigned int)inData->at(5).int32Data[i];
            usrdata=&_usrdata[0];
        }
        COcStruct* ocStruct=geom_createOctreeFromColorPoints(&inData->at(0).realData[0],inData->at(0).realData.size()/3,&tr,cellS,rgb,usrdata);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0,
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
            C3Vector meshPos(&(inData->at(1).realData[0]));
            C4Vector meshQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector pos;
            pos.clear();
            if ( (inData->size()>=4)&&(inData->at(3).realData.size()>=3) )
                pos.set(&inData->at(3).realData[0]);
            C4Vector quat;
            quat.setIdentity();
            if ( (inData->size()>=5)&&(inData->at(4).realData.size()>=4) )
            {
                quat(0)=inData->at(4).realData[3];
                quat(1)=inData->at(4).realData[0];
                quat(2)=inData->at(4).realData[1];
                quat(3)=inData->at(4).realData[2];
            }
            C7Vector tr(quat,pos);
            simReal cellS=simReal(0.05);
            if ( (inData->size()>=6)&&(inData->at(5).realData.size()==1) )
                cellS=inData->at(5).realData[0];
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0,
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
            C3Vector ocPos(&(inData->at(1).realData[0]));
            C4Vector ocQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector pos;
            pos.clear();
            if ( (inData->size()>=4)&&(inData->at(3).realData.size()>=3) )
                pos.set(&inData->at(3).realData[0]);
            C4Vector quat;
            quat.setIdentity();
            if ( (inData->size()>=5)&&(inData->at(4).realData.size()>=4) )
            {
                quat(0)=inData->at(4).realData[3];
                quat(1)=inData->at(4).realData[0];
                quat(2)=inData->at(4).realData[1];
                quat(3)=inData->at(4).realData[2];
            }
            C7Vector tr(quat,pos);
            simReal cellS=simReal(0.05);
            if ( (inData->size()>=6)&&(inData->at(5).realData.size()==1) )
                cellS=inData->at(5).realData[0];
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
    sim_script_arg_real,0,
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
            geom_scaleOctree(it->second,inData->at(1).realData[0]);
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
            std::vector<simReal> data;
            std::vector<unsigned int> usrData;
            geom_getOctreeVoxelData(it->second,data,&usrData);
            std::vector<simReal> pos;
            std::vector<int> rgb;
            for (size_t i=0;i<data.size()/6;i++)
            {
                pos.push_back(data[6*i+0]);
                pos.push_back(data[6*i+1]);
                pos.push_back(data[6*i+2]);
                rgb.push_back(int(data[6*i+3]*simReal(255.1)));
                rgb.push_back(int(data[6*i+4]*simReal(255.1)));
                rgb.push_back(int(data[6*i+5]*simReal(255.1)));
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
    sim_script_arg_real|sim_lua_arg_table,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
};

void LUA_GETTRANSFORMEDPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_GETTRANSFORMEDPOINTS,inArgs_GETTRANSFORMEDPOINTS[0],LUA_GETTRANSFORMEDPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        std::vector<simReal> outData;
        outData.resize(inData->at(0).realData.size());
        C3Vector pos(&(inData->at(1).realData[0]));
        C4Vector q(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
        C7Vector tr(q,pos);
        for (size_t i=0;i<inData->at(0).realData.size()/3;i++)
        {
            C3Vector v(&inData->at(0).realData[0]+3*i);
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
    sim_script_arg_real|sim_lua_arg_table,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // cell size
    sim_script_arg_int32,0, // max points in cell
    sim_script_arg_int32|sim_lua_arg_table,3, // color
    sim_script_arg_real,0, // proximity tolerance
};

void LUA_CREATEPTCLOUDFROMPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMPOINTS,inArgs_CREATEPTCLOUDFROMPOINTS[0]-6,LUA_CREATEPTCLOUDFROMPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).realData.size()>=3) )
            pos.set(&inData->at(1).realData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).realData.size()>=4) )
        {
            quat(0)=inData->at(2).realData[3];
            quat(1)=inData->at(2).realData[0];
            quat(2)=inData->at(2).realData[1];
            quat(3)=inData->at(2).realData[2];
        }
        C7Vector tr(quat,pos);
        simReal cellS=simReal(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).realData.size()==1) )
            cellS=inData->at(3).realData[0];
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
        simReal proxTol=simReal(0.005);
        if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
            proxTol=inData->at(6).realData[0];
        CPcStruct* pcStruct=geom_createPtcloudFromPoints(&inData->at(0).realData[0],inData->at(0).realData.size()/3,&tr,cellS,maxPts,rgb,proxTol);
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
    sim_script_arg_real|sim_lua_arg_table,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // cell size
    sim_script_arg_int32,0, // max points in cell
    sim_script_arg_int32|sim_lua_arg_table,0, // color
    sim_script_arg_real,0, // proximity tolerance
};

void LUA_CREATEPTCLOUDFROMCOLORPOINTS_CALLBACK(SScriptCallBack* p)
{
    CScriptFunctionData D;
    if (D.readDataFromStack(p->stackID,inArgs_CREATEPTCLOUDFROMCOLORPOINTS,inArgs_CREATEPTCLOUDFROMCOLORPOINTS[0]-5,LUA_CREATEPTCLOUDFROMCOLORPOINTS_COMMAND))
    {
        std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
        C3Vector pos;
        pos.clear();
        if ( (inData->size()>=2)&&(inData->at(1).realData.size()>=3) )
            pos.set(&inData->at(1).realData[0]);
        C4Vector quat;
        quat.setIdentity();
        if ( (inData->size()>=3)&&(inData->at(2).realData.size()>=4) )
        {
            quat(0)=inData->at(2).realData[3];
            quat(1)=inData->at(2).realData[0];
            quat(2)=inData->at(2).realData[1];
            quat(3)=inData->at(2).realData[2];
        }
        C7Vector tr(quat,pos);
        simReal cellS=simReal(0.05);
        if ( (inData->size()>=4)&&(inData->at(3).realData.size()==1) )
            cellS=inData->at(3).realData[0];
        int maxPts=20;
        if ( (inData->size()>=5)&&(inData->at(4).int32Data.size()==1) )
            maxPts=inData->at(4).int32Data[0];
        unsigned char* rgb=nullptr;
        std::vector<unsigned char> _rgb;
        if ( (inData->size()>=6)&&(inData->at(5).int32Data.size()>=inData->at(0).realData.size()) )
        {
            _rgb.resize(inData->at(5).int32Data.size());
            for (size_t i=0;i<inData->at(5).int32Data.size();i++)
                _rgb[i]=(unsigned char)inData->at(5).int32Data[i];
            rgb=&_rgb[0];
        }
        simReal proxTol=simReal(0.005);
        if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
            proxTol=inData->at(6).realData[0];
        CPcStruct* pcStruct=geom_createPtcloudFromColorPoints(&inData->at(0).realData[0],inData->at(0).realData.size()/3,&tr,cellS,maxPts,rgb,proxTol);
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
    sim_script_arg_real,0,
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
            geom_scalePtcloud(it->second,inData->at(1).realData[0]);
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
    sim_script_arg_real,0,
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
            simReal prop=simOne;
            if ( (inData->size()>=2)&&(inData->at(1).realData.size()==1) )
                prop=inData->at(1).realData[0];
            std::vector<simReal> data;
            geom_getPtcloudPoints(it->second,data,prop);
            std::vector<simReal> pos;
            std::vector<int> rgb;
            for (size_t i=0;i<data.size()/6;i++)
            {
                pos.push_back(data[6*i+0]);
                pos.push_back(data[6*i+1]);
                pos.push_back(data[6*i+2]);
                rgb.push_back(int(data[6*i+3]*simReal(255.1)));
                rgb.push_back(int(data[6*i+4]*simReal(255.1)));
                rgb.push_back(int(data[6*i+5]*simReal(255.1)));
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_int32,0,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector ptcloud1Pos(&(inData->at(1).realData[0]));
            C4Vector ptcloud1Q(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector ptcloud2Pos(&(inData->at(4).realData[0]));
            C4Vector ptcloud2Q(inData->at(5).realData[3],inData->at(5).realData[0],inData->at(5).realData[1],inData->at(5).realData[2]);
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
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
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector ptcloudPos(&(inData->at(1).realData[0]));
            C4Vector ptcloudQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector triPt1(&(inData->at(3).realData[0]));
            C3Vector triPt2(&(inData->at(4).realData[0]));
            C3Vector triPt3(&(inData->at(5).realData[0]));
            simReal dist=REAL_MAX;
            if ( (inData->size()>=7)&&(inData->at(6).realData.size()==1) )
                dist=inData->at(6).realData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=8)&&(inData->at(7).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(7).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudTriangleDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),triPt1,triPt2-triPt1,triPt3-triPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector ptcloudPos(&(inData->at(1).realData[0]));
            C4Vector ptcloudQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector segPt1(&(inData->at(3).realData[0]));
            C3Vector segPt2(&(inData->at(4).realData[0]));
            simReal dist=REAL_MAX;
            if ( (inData->size()>=6)&&(inData->at(5).realData.size()==1) )
                dist=inData->at(5).realData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=7)&&(inData->at(6).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(6).doubleData[0];
            C3Vector distPt1,distPt2;
            bool smaller=geom_getPtcloudSegmentDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),segPt1,segPt2-segPt1,dist,&distPt1,&distPt2,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt1.data,distPt1.data+3);
                D.pushOutData(CScriptFunctionDataItem(pp1));
                std::vector<simReal> pp2(distPt2.data,distPt2.data+3);
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
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real|sim_lua_arg_table,4,
    sim_script_arg_real|sim_lua_arg_table,3,
    sim_script_arg_real,0, // threshold, default is REAL_MAX
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
            C3Vector ptcloudPos(&(inData->at(1).realData[0]));
            C4Vector ptcloudQ(inData->at(2).realData[3],inData->at(2).realData[0],inData->at(2).realData[1],inData->at(2).realData[2]);
            C3Vector point(&(inData->at(3).realData[0]));
            simReal dist=REAL_MAX;
            if ( (inData->size()>=5)&&(inData->at(4).realData.size()==1) )
                dist=inData->at(4).realData[0];
            unsigned long long int cache=0;
            if ( (inData->size()>=6)&&(inData->at(5).doubleData.size()==1) )
                cache=(unsigned long long int)inData->at(5).doubleData[0];
            C3Vector distPt;
            bool smaller=geom_getPtcloudPointDistanceIfSmaller(it->second,C7Vector(ptcloudQ,ptcloudPos),point,dist,&distPt,&cache);
            D.pushOutData(CScriptFunctionDataItem(dist));
            if (smaller)
            {
                std::vector<simReal> pp1(distPt.data,distPt.data+3);
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
        outputMsg(sim_verbosity_errors,"simExtGeometric plugin error: could not find or correctly load the CoppeliaSim library. Cannot start 'Geometric' plugin.");
        return(0);
    }
    if (getSimProcAddresses(simLib)==0)
    {
        outputMsg(sim_verbosity_errors,"simExtGeometric plugin error: could not find all required functions in the CoppeliaSim library. Cannot start 'Geometric' plugin.");
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

    simRegisterScriptCallbackFunction(LUA_GETMESHMESHDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETMESHMESHDISTANCE_COMMAND,"(\nnumber mesh1Handle,table_3 mesh1Pos,table_4 mesh1Quaternion,\nnumber mesh2Handle,table_3 mesh2Pos,table_4 mesh2Quaternion,\nnumber distanceThreshold=REAL_MAX,table_2 cache=nil)"),LUA_GETMESHMESHDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHOCTREEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETMESHOCTREEDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\nnumber distanceThreshold=REAL_MAX,table_2 cache=nil)"),LUA_GETMESHOCTREEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHPTCLOUDDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETMESHPTCLOUDDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\nnumber distanceThreshold=REAL_MAX,table_2 cache=nil)"),LUA_GETMESHPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHTRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETMESHTRIANGLEDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETMESHTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHSEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETMESHSEGMENTDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 segmentPt1,table_3 segmentPt2,\nnumber distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETMESHSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETMESHPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt,number cache=",LUA_GETMESHPOINTDISTANCE_COMMAND,"(\nnumber meshHandle,table_3 meshPos,table_4 meshQuaternion,\ntable_3 point,number distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETMESHPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETOCTREEOCTREEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETOCTREEOCTREEDISTANCE_COMMAND,"(\nnumber octree1Handle,table_3 octree1Pos,table_4 octree1Quaternion,\nnumber octree2Handle,table_3 octree2Pos,table_4 octree2Quaternion,\nnumber distanceThreshold=REAL_MAX,table_2 cache=nil)"),LUA_GETOCTREEOCTREEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETOCTREEPTCLOUDDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\nnumber distanceThreshold=REAL_MAX,table_2 cache=nil)"),LUA_GETOCTREEPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREETRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETOCTREETRIANGLEDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETOCTREETRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREESEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETOCTREESEGMENTDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 segPt1,table_3 segPt2,\nnumber distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETOCTREESEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETOCTREEPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt,number cache=",LUA_GETOCTREEPOINTDISTANCE_COMMAND,"(\nnumber octreeHandle,table_3 octreePos,table_4 octreeQuaternion,\ntable_3 point,number distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETOCTREEPOINTDISTANCE_CALLBACK);

    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,table_2 cache=",LUA_GETPTCLOUDPTCLOUDDISTANCE_COMMAND,"(\nnumber ptcloud1Handle,table_3 ptcloud1Pos,table_4 ptcloud1Quaternion,\nnumber ptcloud2Handle,table_3 ptcloud2Pos,table_4 ptcloud2Quaternion,\nnumber distanceThreshold=REAL_MAX,table_2 cache=nil)"),LUA_GETPTCLOUDPTCLOUDDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETPTCLOUDTRIANGLEDISTANCE_COMMAND,"(\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_3 triPt1,table_3 triPt2,table_3 triPt3,\nnumber distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETPTCLOUDTRIANGLEDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt1,table_3 distSegPt2,number cache=",LUA_GETPTCLOUDSEGMENTDISTANCE_COMMAND,"(\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_3 segPt1,table_3 segPt2,\nnumber distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETPTCLOUDSEGMENTDISTANCE_CALLBACK);
    simRegisterScriptCallbackFunction(LUA_GETPTCLOUDPOINTDISTANCE_COMMAND_PLUGIN,strConCat("number dist,table_3 distSegPt,number cache=",LUA_GETPTCLOUDPOINTDISTANCE_COMMAND,"(\nnumber ptcloudHandle,table_3 ptcloudPos,table_4 ptcloudQuaternion,\ntable_3 point,number distanceThreshold=REAL_MAX,number cache=-1)"),LUA_GETPTCLOUDPOINTDISTANCE_CALLBACK);

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

SIM_DLLEXPORT void* geomPlugin_createMesh(const simReal* vertices,int verticesSize,const int* indices,int indicesSize,const simReal meshOrigin[7],simReal triangleEdgeMaxLength,int maxTrianglesInBoundingBox)
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

SIM_DLLEXPORT void geomPlugin_scaleMesh(void* meshObbStruct,simReal scalingFactor)
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

SIM_DLLEXPORT simReal geomPlugin_getMeshRootObbVolume(const void* meshObbStruct)
{
    return(geom_getMeshRootObbVolume((const CObbStruct*)meshObbStruct));
}

SIM_DLLEXPORT void* geomPlugin_createOctreeFromPoints(const simReal* points,int pointCnt,const simReal octreeOrigin[7],simReal cellS,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setInternalData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromPoints(points,pointCnt,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromColorPoints(const simReal* points,int pointCnt,const simReal octreeOrigin[7],simReal cellS,const unsigned char* rgbData,const unsigned int* usrData)
{
    C7Vector tr;
    tr.setIdentity();
    if (octreeOrigin!=nullptr)
        tr.setInternalData(octreeOrigin);
    COcStruct* retVal=geom_createOctreeFromColorPoints(points,pointCnt,&tr,cellS,rgbData,usrData);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createOctreeFromMesh(const void* meshObbStruct,const simReal meshTransformation[7],const simReal octreeOrigin[7],simReal cellS,const unsigned char rgbData[3],unsigned int usrData)
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
SIM_DLLEXPORT void* geomPlugin_createOctreeFromOctree(const void* otherOctreeStruct,const simReal otherOctreeTransformation[7],const simReal newOctreeOrigin[7],simReal newOctreeCellS,const unsigned char rgbData[3],unsigned int usrData)
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
SIM_DLLEXPORT void geomPlugin_scaleOctree(void* ocStruct,simReal f)
{
    geom_scaleOctree((COcStruct*)ocStruct,f);
}
SIM_DLLEXPORT void geomPlugin_destroyOctree(void* ocStruct)
{
    geom_destroyOctree((COcStruct*)ocStruct);
}
SIM_DLLEXPORT simReal* geomPlugin_getOctreeVoxelData(const void* ocStruct,int* voxelCount)
{ // voxel position + rgb
    std::vector<simReal> data;
    geom_getOctreeVoxelData((const COcStruct*)ocStruct,data,nullptr);
    simReal* retVal=new simReal[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    voxelCount[0]=((int)data.size())/6;
    return(retVal);
}
SIM_DLLEXPORT unsigned int* geomPlugin_getOctreeUserData(const void* ocStruct,int* voxelCount)
{
    std::vector<simReal> data;
    std::vector<unsigned int> usrData;
    geom_getOctreeVoxelData((const COcStruct*)ocStruct,data,&usrData);
    unsigned int* retVal=new unsigned int[usrData.size()];
    for (size_t i=0;i<usrData.size();i++)
        retVal[i]=usrData[i];
    voxelCount[0]=(int)usrData.size();
    return(retVal);
}
SIM_DLLEXPORT simReal* geomPlugin_getOctreeCornersFromOctree(const void* ocStruct,int* pointCount)
{
    std::vector<simReal> data;
    geom_getOctreeCornersFromOctree((const COcStruct*)ocStruct,data);
    simReal* retVal=new simReal[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/3;
    return(retVal);
}

SIM_DLLEXPORT void geomPlugin_insertPointsIntoOctree(void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCnt,const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    geom_insertPointsIntoOctree((COcStruct*)ocStruct,tr,points,pointCnt,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoOctree(void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCnt,const unsigned char* rgbData,const unsigned int* usrData)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    geom_insertColorPointsIntoOctree((COcStruct*)ocStruct,tr,points,pointCnt,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertMeshIntoOctree(void* ocStruct,const simReal octreeTransformation[7],const void* obbStruct,const simReal meshTransformation[7],const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C7Vector meshTr;
    meshTr.setInternalData(meshTransformation);
    geom_insertMeshIntoOctree((COcStruct*)ocStruct,tr,(const CObbStruct*)obbStruct,meshTr,rgbData,usrData);
}
SIM_DLLEXPORT void geomPlugin_insertOctreeIntoOctree(void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7],const unsigned char rgbData[3],unsigned int usrData)
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    geom_insertOctreeIntoOctree((COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,rgbData,usrData);
}
SIM_DLLEXPORT bool geomPlugin_removePointsFromOctree(void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCnt)
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    bool retVal=geom_removePointsFromOctree((COcStruct*)ocStruct,tr,points,pointCnt);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeMeshFromOctree(void* ocStruct,const simReal octreeTransformation[7],const void* obbStruct,const simReal meshTransformation[7])
{
    C7Vector tr;
    tr.setInternalData(octreeTransformation);
    C7Vector meshTr;
    meshTr.setInternalData(meshTransformation);
    bool retVal=geom_removeMeshFromOctree((COcStruct*)ocStruct,tr,(const CObbStruct*)obbStruct,meshTr);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromOctree(void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7])
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    bool retVal=geom_removeOctreeFromOctree((COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2);
    return(retVal);
}



SIM_DLLEXPORT void* geomPlugin_createPtcloudFromPoints(const simReal* points,int pointCnt,const simReal ptcloudOrigin[7],simReal cellS,int maxPointCnt,const unsigned char rgbData[3],simReal proximityTol)
{
    C7Vector tr;
    tr.setIdentity();
    if (ptcloudOrigin!=nullptr)
        tr.setInternalData(ptcloudOrigin);
    void* retVal=geom_createPtcloudFromPoints(points,pointCnt,&tr,cellS,maxPointCnt,rgbData,proximityTol);
    return(retVal);
}
SIM_DLLEXPORT void* geomPlugin_createPtcloudFromColorPoints(const simReal* points,int pointCnt,const simReal ptcloudOrigin[7],simReal cellS,int maxPointCnt,const unsigned char* rgbData,simReal proximityTol)
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
SIM_DLLEXPORT void geomPlugin_scalePtcloud(void* pcStruct,simReal f)
{
    geom_scalePtcloud((CPcStruct*)pcStruct,f);
}
SIM_DLLEXPORT void geomPlugin_destroyPtcloud(void* pcStruct)
{
    geom_destroyPtcloud((CPcStruct*)pcStruct);
}
SIM_DLLEXPORT simReal* geomPlugin_getPtcloudPoints(const void* pcStruct,int* pointCount,simReal prop)
{
    std::vector<simReal> data;
    geom_getPtcloudPoints((const CPcStruct*)pcStruct,data,prop);
    simReal* retVal=new simReal[data.size()];
    for (size_t i=0;i<data.size();i++)
        retVal[i]=data[i];
    pointCount[0]=((int)data.size())/6;
    return(retVal);
}
SIM_DLLEXPORT simReal* geomPlugin_getPtcloudOctreeCorners(const void* pcStruct,int* pointCount)
{
    std::vector<simReal> data;
    geom_getPtcloudOctreeCorners((const CPcStruct*)pcStruct,data);
    simReal* retVal=new simReal[data.size()];
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

SIM_DLLEXPORT void geomPlugin_insertPointsIntoPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,const unsigned char rgbData[3],simReal proximityTol)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    geom_insertPointsIntoPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,rgbData,proximityTol);
}
SIM_DLLEXPORT void geomPlugin_insertColorPointsIntoPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,const unsigned char* rgbData,simReal proximityTol)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    geom_insertColorPointsIntoPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,rgbData,proximityTol);
}
SIM_DLLEXPORT bool geomPlugin_removePointsFromPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,simReal proximityTol,int* countRemoved)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    bool retVal=geom_removePointsFromPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,proximityTol,countRemoved);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_removeOctreeFromPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const void* ocStruct,const simReal octreeTransformation[7],int* countRemoved)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    C7Vector octreeTr;
    octreeTr.setInternalData(octreeTransformation);
    bool retVal=geom_removeOctreeFromPtcloud((CPcStruct*)pcStruct,tr,(const COcStruct*)ocStruct,octreeTr,countRemoved);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_intersectPointsWithPtcloud(void* pcStruct,const simReal ptcloudTransformation[7],const simReal* points,int pointCnt,simReal proximityTol)
{
    C7Vector tr;
    tr.setInternalData(ptcloudTransformation);
    bool retVal=geom_intersectPointsWithPtcloud((CPcStruct*)pcStruct,tr,points,pointCnt,proximityTol);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getMeshMeshCollision(const void* mesh1ObbStruct,const simReal mesh1Transformation[7],const void* mesh2ObbStruct,const simReal mesh2Transformation[7],simReal** intersections,int* intersectionsSize,int* mesh1Caching,int* mesh2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(mesh1Transformation);
    C7Vector tr2;
    tr2.setInternalData(mesh2Transformation);
    std::vector<simReal> _intersections;
    std::vector<simReal>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshMeshCollision((const CObbStruct*)mesh1ObbStruct,tr1,(const CObbStruct*)mesh2ObbStruct,tr2,_ints,mesh1Caching,mesh2Caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new simReal[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeCollision(const void* meshObbStruct,const simReal meshTransformation[7],const void* ocStruct,const simReal octreeTransformation[7],int* meshCaching,unsigned long long int* ocCaching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C7Vector tr2;
    tr2.setInternalData(octreeTransformation);
    bool retVal=geom_getMeshOctreeCollision((const CObbStruct*)meshObbStruct,tr1,(const COcStruct*)ocStruct,tr2,meshCaching,ocCaching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleCollision(const void* meshObbStruct,const simReal meshTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal** intersections,int* intersectionsSize,int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    std::vector<simReal> _intersections;
    std::vector<simReal>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshTriangleCollision((const CObbStruct*)meshObbStruct,tr1,_p,_v,_w,_ints,caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new simReal[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentCollision(const void* meshObbStruct,const simReal meshTransformation[7],const simReal segmentExtremity[3],const simReal segmentVector[3],simReal** intersections,int* intersectionsSize,int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(meshTransformation);
    C3Vector _segmentExtremity(segmentExtremity);
    C3Vector _segmentVector(segmentVector);
    std::vector<simReal> _intersections;
    std::vector<simReal>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getMeshSegmentCollision((const CObbStruct*)meshObbStruct,tr1,_segmentExtremity,_segmentVector,_ints,caching);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new simReal[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeCollision(const void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching)
{
    C7Vector tr1;
    tr1.setInternalData(octree1Transformation);
    C7Vector tr2;
    tr2.setInternalData(octree2Transformation);
    bool retVal=geom_getOctreeOctreeCollision((const COcStruct*)oc1Struct,tr1,(const COcStruct*)oc2Struct,tr2,oc1Caching,oc2Caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudCollision(const void* ocStruct,const simReal octreeTransformation[7],const void* pcStruct,const simReal ptcloudTransformation[7],unsigned long long int* ocCaching,unsigned long long int* pcCaching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C7Vector tr2;
    tr2.setInternalData(ptcloudTransformation);
    bool retVal=geom_getOctreePtcloudCollision((const COcStruct*)ocStruct,tr1,(const CPcStruct*)pcStruct,tr2,ocCaching,pcCaching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    bool retVal=geom_getOctreeTriangleCollision((const COcStruct*)ocStruct,tr1,_p,_v,_w,caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal segmentExtremity[3],const simReal segmentVector[3],unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C3Vector _segmentExtremity(segmentExtremity);
    C3Vector _segmentVector(segmentVector);
    bool retVal=geom_getOctreeSegmentCollision((const COcStruct*)ocStruct,tr1,_segmentExtremity,_segmentVector,caching);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getOctreePointsCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal* points,int pointCount)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    bool retVal=geom_getOctreePointsCollision((const COcStruct*)ocStruct,tr1,points,pointCount);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getOctreePointCollision(const void* ocStruct,const simReal octreeTransformation[7],const simReal point[3],unsigned int* usrData,unsigned long long int* caching)
{
    C7Vector tr1;
    tr1.setInternalData(octreeTransformation);
    C3Vector _point(point);
    bool retVal=geom_getOctreePointCollision((const COcStruct*)ocStruct,tr1,_point,usrData,caching);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getBoxBoxCollision(const simReal box1Transformation[7],const simReal box1HalfSize[3],const simReal box2Transformation[7],const simReal box2HalfSize[3],bool boxesAreSolid)
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
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleCollision(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal p[3],const simReal v[3],const simReal w[3])
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
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentCollision(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal segmentEndPoint[3],const simReal segmentVector[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    bool retVal=geom_getBoxSegmentCollision(tr,bhs,boxIsSolid,_segmentEndPoint,_segmentVector);
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getBoxPointCollision(const simReal boxTransformation[7],const simReal boxHalfSize[3],const simReal point[3])
{
    C7Vector tr;
    tr.setInternalData(boxTransformation);
    C3Vector bhs(boxHalfSize);
    C3Vector _point(point);
    bool retVal=geom_getBoxPointCollision(tr,bhs,_point);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleCollision(const simReal p1[3],const simReal v1[3],const simReal w1[3],const simReal p2[3],const simReal v2[3],const simReal w2[3],simReal** intersections,int* intersectionsSize)
{
    C3Vector _p1(p1);
    C3Vector _v1(v1);
    C3Vector _w1(w1);
    C3Vector _p2(p2);
    C3Vector _v2(v2);
    C3Vector _w2(w2);
    std::vector<simReal> _intersections;
    std::vector<simReal>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getTriangleTriangleCollision(_p1,_v1,_w1,_p2,_v2,_w2,_ints);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new simReal[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentCollision(const simReal p[3],const simReal v[3],const simReal w[3],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal** intersections,int* intersectionsSize)
{
    C3Vector _p(p);
    C3Vector _v(v);
    C3Vector _w(w);
    C3Vector _segmentEndPoint(segmentEndPoint);
    C3Vector _segmentVector(segmentVector);
    std::vector<simReal> _intersections;
    std::vector<simReal>* _ints=nullptr;
    if (intersections!=nullptr)
        _ints=&_intersections;
    bool retVal=geom_getTriangleSegmentCollision(_p,_v,_w,_segmentEndPoint,_segmentVector,_ints);
    if (retVal&&(intersections!=nullptr))
    {
        intersections[0]=new simReal[_intersections.size()];
        intersectionsSize[0]=int(_intersections.size());
        for (size_t i=0;i<_intersections.size();i++)
            intersections[0][i]=_intersections[i];
    }
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getMeshMeshDistanceIfSmaller(const void* mesh1ObbStruct,const simReal mesh1Transformation[7],const void* mesh2ObbStruct,const simReal mesh2Transformation[7],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3],int* mesh1Caching,int* mesh2Caching)
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
SIM_DLLEXPORT bool geomPlugin_getMeshOctreeDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const void* ocStruct,const simReal octreeTransformation[7],simReal* dist,simReal meshMinDistPt[3],simReal ocMinDistPt[3],int* meshCaching,unsigned long long int* ocCaching)
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
SIM_DLLEXPORT bool geomPlugin_getMeshPtcloudDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const void* pcStruct,const simReal pcTransformation[7],simReal* dist,simReal meshMinDistPt[3],simReal pcMinDistPt[3],int* meshCaching,unsigned long long int* pcCaching)
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
SIM_DLLEXPORT bool geomPlugin_getMeshTriangleDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3],int* caching)
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
SIM_DLLEXPORT bool geomPlugin_getMeshSegmentDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3],int* caching)
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
SIM_DLLEXPORT bool geomPlugin_getMeshPointDistanceIfSmaller(const void* meshObbStruct,const simReal meshTransformation[7],const simReal point[3],simReal* dist,simReal minDistSegPt[3],int* caching)
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

SIM_DLLEXPORT bool geomPlugin_getOctreeOctreeDistanceIfSmaller(const void* oc1Struct,const simReal octree1Transformation[7],const void* oc2Struct,const simReal octree2Transformation[7],simReal* dist,simReal oc1MinDistPt[3],simReal oc2MinDistPt[3],unsigned long long int* oc1Caching,unsigned long long int* oc2Caching)
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
SIM_DLLEXPORT bool geomPlugin_getOctreePtcloudDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const void* pcStruct,const simReal pcTransformation[7],simReal* dist,simReal ocMinDistPt[3],simReal pcMinDistPt[3],unsigned long long int* ocCaching,unsigned long long int* pcCaching)
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
SIM_DLLEXPORT bool geomPlugin_getOctreeTriangleDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal ocMinDistPt[3],simReal triMinDistPt[3],unsigned long long int* ocCaching)
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
SIM_DLLEXPORT bool geomPlugin_getOctreeSegmentDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal ocMinDistPt[3],simReal segMinDistPt[3],unsigned long long int* ocCaching)
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
SIM_DLLEXPORT bool geomPlugin_getOctreePointDistanceIfSmaller(const void* ocStruct,const simReal octreeTransformation[7],const simReal point[3],simReal* dist,simReal ocMinDistPt[3],unsigned long long int* ocCaching)
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

SIM_DLLEXPORT bool geomPlugin_getPtcloudPtcloudDistanceIfSmaller(const void* pc1Struct,const simReal pc1Transformation[7],const void* pc2Struct,const simReal pc2Transformation[7],simReal* dist,simReal* pc1MinDistPt,simReal* pc2MinDistPt,unsigned long long int* pc1Caching,unsigned long long int* pc2Caching)
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
SIM_DLLEXPORT bool geomPlugin_getPtcloudTriangleDistanceIfSmaller(const void* pcStruct,const simReal pcTransformation[7],const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal* pcMinDistPt,simReal* triMinDistPt,unsigned long long int* pcCaching)
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
SIM_DLLEXPORT bool geomPlugin_getPtcloudSegmentDistanceIfSmaller(const void* pcStruct,const simReal pcTransformation[7],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal* pcMinDistPt,simReal* segMinDistPt,unsigned long long int* pcCaching)
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
SIM_DLLEXPORT bool geomPlugin_getPtcloudPointDistanceIfSmaller(const void* pcStruct,const simReal pcTransformation[7],const simReal point[3],simReal* dist,simReal* pcMinDistPt,unsigned long long int* pcCaching)
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

SIM_DLLEXPORT simReal geomPlugin_getApproxBoxBoxDistance(const simReal box1Transformation[7],const simReal box1HalfSize[3],const simReal box2Transformation[7],const simReal box2HalfSize[3])
{
    C7Vector tr1;
    tr1.setInternalData(box1Transformation);
    C7Vector tr2;
    tr2.setInternalData(box2Transformation);
    C3Vector _b1hs(box1HalfSize);
    C3Vector _b2hs(box2HalfSize);
    simReal retVal=geom_getApproxBoxBoxDistance(tr1,_b1hs,tr2,_b2hs);
    return(retVal);
}

SIM_DLLEXPORT bool geomPlugin_getBoxBoxDistanceIfSmaller(const simReal box1Transformation[7],const simReal box1HalfSize[3],const simReal box2Transformation[7],const simReal box2HalfSize[3],bool boxesAreSolid,simReal* dist,simReal distSegPt1[3],simReal distSegPt2[3])
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
SIM_DLLEXPORT bool geomPlugin_getBoxTriangleDistanceIfSmaller(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,simReal distSegPt1[3],simReal distSegPt2[3])
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
SIM_DLLEXPORT bool geomPlugin_getBoxSegmentDistanceIfSmaller(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal distSegPt1[3],simReal distSegPt2[3])
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
SIM_DLLEXPORT bool geomPlugin_getBoxPointDistanceIfSmaller(const simReal boxTransformation[7],const simReal boxHalfSize[3],bool boxIsSolid,const simReal point[3],simReal* dist,simReal distSegPt1[3])
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

SIM_DLLEXPORT bool geomPlugin_getTriangleTriangleDistanceIfSmaller(const simReal p1[3],const simReal v1[3],const simReal w1[3],const simReal p2[3],const simReal v2[3],const simReal w2[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3])
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
SIM_DLLEXPORT bool geomPlugin_getTriangleSegmentDistanceIfSmaller(const simReal p[3],const simReal v[3],const simReal w[3],const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3])
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
SIM_DLLEXPORT bool geomPlugin_getTrianglePointDistanceIfSmaller(const simReal p[3],const simReal v[3],const simReal w[3],const simReal point[3],simReal* dist,simReal minDistSegPt[3])
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

SIM_DLLEXPORT bool geomPlugin_getSegmentSegmentDistanceIfSmaller(const simReal segment1EndPoint[3],const simReal segment1Vector[3],const simReal segment2EndPoint[3],const simReal segment2Vector[3],simReal* dist,simReal minDistSegPt1[3],simReal minDistSegPt2[3])
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
SIM_DLLEXPORT bool geomPlugin_getSegmentPointDistanceIfSmaller(const simReal segmentEndPoint[3],const simReal segmentVector[3],const simReal point[3],simReal* dist,simReal minDistSegPt[3])
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

SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectMeshIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const void* obbStruct,const simReal meshTransformationRelative[7],simReal* dist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3])
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
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectOctreeIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const void* ocStruct,const simReal octreeTransformationRelative[7],simReal* dist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3])
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
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectPtcloudIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const void* pcStruct,const simReal ptcloudTransformationRelative[7],simReal* dist,bool fast,simReal detectPt[3])
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
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectTriangleIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const simReal p[3],const simReal v[3],const simReal w[3],simReal* dist,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3])
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
SIM_DLLEXPORT bool geomPlugin_volumeSensorDetectSegmentIfSmaller(const simReal* planesIn,int planesInSize,const simReal* planesOut,int planesOutSize,const simReal segmentEndPoint[3],const simReal segmentVector[3],simReal* dist,simReal maxAngle,simReal detectPt[3])
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

SIM_DLLEXPORT bool geomPlugin_raySensorDetectMeshIfSmaller(const simReal rayStart[3],const simReal rayVect[3],const void* obbStruct,const simReal meshTransformationRelative[7],simReal* dist,simReal forbiddenDist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3],bool* forbiddenDistTouched)
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
SIM_DLLEXPORT bool geomPlugin_raySensorDetectOctreeIfSmaller(const simReal rayStart[3],const simReal rayVect[3],const void* ocStruct,const simReal octreeTransformationRelative[7],simReal* dist,simReal forbiddenDist,bool fast,bool frontDetection,bool backDetection,simReal maxAngle,simReal detectPt[3],simReal triN[3],bool* forbiddenDistTouched)
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

SIM_DLLEXPORT bool geomPlugin_isPointInVolume(const simReal* planesIn,int planesInSize,const simReal point[3])
{
    bool retVal=geom_isPointInVolume(planesIn,planesInSize,C3Vector(point));
    return(retVal);
}
