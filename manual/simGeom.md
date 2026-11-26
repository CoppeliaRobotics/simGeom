# Geom plugin API reference

All units, unless otherwise indicated, are specified in meters and radians. Note that mesh-, point cloud- and OC tree-handles mentioned in this reference have no direct relation with CoppeliaSim shape-, point cloud- and OC tree-handles: meshes, point cloud and octrees mentioned in this API reference are internal representations that are exclusively created, manipulated and destroyed with below API functions. Below API functions are a sub-set of their C++ API counterpart.

## Alphabetical list

- [simGeom.copyMesh](#copyMesh)
- [simGeom.copyOctree](#copyOctree)
- [simGeom.copyPtcloud](#copyPtcloud)
- [simGeom.createMesh](#createMesh)
- [simGeom.createMeshFromSerializationData](#createMeshFromSerializationData)
- [simGeom.createOctreeFromColorPoints](#createOctreeFromColorPoints)
- [simGeom.createOctreeFromMesh](#createOctreeFromMesh)
- [simGeom.createOctreeFromOctree](#createOctreeFromOctree)
- [simGeom.createOctreeFromPoints](#createOctreeFromPoints)
- [simGeom.createOctreeFromSerializationData](#createOctreeFromSerializationData)
- [simGeom.createPtcloudFromColorPoints](#createPtcloudFromColorPoints)
- [simGeom.createPtcloudFromPoints](#createPtcloudFromPoints)
- [simGeom.createPtcloudFromSerializationData](#createPtcloudFromSerializationData)
- [simGeom.destroyMesh](#destroyMesh)
- [simGeom.destroyOctree](#destroyOctree)
- [simGeom.destroyPtcloud](#destroyPtcloud)
- [simGeom.getBoxBoxDistance](#getBoxBoxDistance)
- [simGeom.getBoxPointDistance](#getBoxPointDistance)
- [simGeom.getBoxSegmentDistance](#getBoxSegmentDistance)
- [simGeom.getBoxTriangleDistance](#getBoxTriangleDistance)
- [simGeom.getMeshMeshCollision](#getMeshMeshCollision)
- [simGeom.getMeshMeshDistance](#getMeshMeshDistance)
- [simGeom.getMeshOctreeCollision](#getMeshOctreeCollision)
- [simGeom.getMeshOctreeDistance](#getMeshOctreeDistance)
- [simGeom.getMeshPointDistance](#getMeshPointDistance)
- [simGeom.getMeshPtcloudDistance](#getMeshPtcloudDistance)
- [simGeom.getMeshSegmentCollision](#getMeshSegmentCollision)
- [simGeom.getMeshSegmentDistance](#getMeshSegmentDistance)
- [simGeom.getMeshSerializationData](#getMeshSerializationData)
- [simGeom.getMeshTriangleCollision](#getMeshTriangleCollision)
- [simGeom.getMeshTriangleDistance](#getMeshTriangleDistance)
- [simGeom.getOctreeOctreeCollision](#getOctreeOctreeCollision)
- [simGeom.getOctreeOctreeDistance](#getOctreeOctreeDistance)
- [simGeom.getOctreePointCollision](#getOctreePointCollision)
- [simGeom.getOctreePointDistance](#getOctreePointDistance)
- [simGeom.getOctreePtcloudCollision](#getOctreePtcloudCollision)
- [simGeom.getOctreePtcloudDistance](#getOctreePtcloudDistance)
- [simGeom.getOctreeSegmentCollision](#getOctreeSegmentCollision)
- [simGeom.getOctreeSegmentDistance](#getOctreeSegmentDistance)
- [simGeom.getOctreeSerializationData](#getOctreeSerializationData)
- [simGeom.getOctreeTriangleCollision](#getOctreeTriangleCollision)
- [simGeom.getOctreeTriangleDistance](#getOctreeTriangleDistance)
- [simGeom.getOctreeVoxels](#getOctreeVoxels)
- [simGeom.getPtcloudPointDistance](#getPtcloudPointDistance)
- [simGeom.getPtcloudPoints](#getPtcloudPoints)
- [simGeom.getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance)
- [simGeom.getPtcloudSegmentDistance](#getPtcloudSegmentDistance)
- [simGeom.getPtcloudSerializationData](#getPtcloudSerializationData)
- [simGeom.getPtcloudTriangleDistance](#getPtcloudTriangleDistance)
- [simGeom.getSegmentPointDistance](#getSegmentPointDistance)
- [simGeom.getSegmentSegmentDistance](#getSegmentSegmentDistance)
- [simGeom.getTransformedPoints](#getTransformedPoints)
- [simGeom.getTrianglePointDistance](#getTrianglePointDistance)
- [simGeom.getTriangleSegmentDistance](#getTriangleSegmentDistance)
- [simGeom.getTriangleTriangleDistance](#getTriangleTriangleDistance)
- [simGeom.scaleMesh](#scaleMesh)
- [simGeom.scaleOctree](#scaleOctree)
- [simGeom.scalePtcloud](#scalePtcloud)

## Mesh creation/destruction/manipulation/info

- [simGeom.createMesh](#createMesh)
- [simGeom.destroyMesh](#destroyMesh)
- [simGeom.copyMesh](#copyMesh)
- [simGeom.scaleMesh](#scaleMesh)
- [simGeom.getMeshSerializationData](#getMeshSerializationData)
- [simGeom.createMeshFromSerializationData](#createMeshFromSerializationData)

## OC-tree creation/destruction/manipulation/info

- [simGeom.createOctreeFromColorPoints](#createOctreeFromColorPoints)
- [simGeom.createOctreeFromMesh](#createOctreeFromMesh)
- [simGeom.createOctreeFromOctree](#createOctreeFromOctree)
- [simGeom.createOctreeFromPoints](#createOctreeFromPoints)
- [simGeom.destroyOctree](#destroyOctree)
- [simGeom.copyOctree](#copyOctree)
- [simGeom.scaleOctree](#scaleOctree)
- [simGeom.getOctreeSerializationData](#getOctreeSerializationData)
- [simGeom.createOctreeFromSerializationData](#createOctreeFromSerializationData)
- [simGeom.getOctreeVoxels](#getOctreeVoxels)

## Point-cloud creation/destruction/manipulation/info

- [simGeom.createPtcloudFromColorPoints](#createPtcloudFromColorPoints)
- [simGeom.createPtcloudFromPoints](#createPtcloudFromPoints)
- [simGeom.destroyPtcloud](#destroyPtcloud)
- [simGeom.copyPtcloud](#copyPtcloud)
- [simGeom.scalePtcloud](#scalePtcloud)
- [simGeom.getPtcloudSerializationData](#getPtcloudSerializationData)
- [simGeom.createPtcloudFromSerializationData](#createPtcloudFromSerializationData)
- [simGeom.getPtcloudPoints](#getPtcloudPoints)

## Mesh collision detection

- [simGeom.getMeshMeshCollision](#getMeshMeshCollision)
- [simGeom.getMeshOctreeCollision](#getMeshOctreeCollision)
- [simGeom.getMeshTriangleCollision](#getMeshTriangleCollision)
- [simGeom.getMeshSegmentCollision](#getMeshSegmentCollision)

## OC-tree collision detection

- [simGeom.getMeshOctreeCollision](#getMeshOctreeCollision)
- [simGeom.getOctreeOctreeCollision](#getOctreeOctreeCollision)
- [simGeom.getOctreePtcloudCollision](#getOctreePtcloudCollision)
- [simGeom.getOctreeTriangleCollision](#getOctreeTriangleCollision)
- [simGeom.getOctreeSegmentCollision](#getOctreeSegmentCollision)
- [simGeom.getOctreePointCollision](#getOctreePointCollision)

## Point-cloud collision detection

- [simGeom.getOctreePtcloudCollision](#getOctreePtcloudCollision)

## Mesh minimum distance calculation

- [simGeom.getMeshMeshDistance](#getMeshMeshDistance)
- [simGeom.getMeshOctreeDistance](#getMeshOctreeDistance)
- [simGeom.getMeshPtcloudDistance](#getMeshPtcloudDistance)
- [simGeom.getMeshTriangleDistance](#getMeshTriangleDistance)
- [simGeom.getMeshSegmentDistance](#getMeshSegmentDistance)
- [simGeom.getMeshPointDistance](#getMeshPointDistance)

## OC-tree minimum distance calculation

- [simGeom.getMeshOctreeDistance](#getMeshOctreeDistance)
- [simGeom.getOctreeOctreeDistance](#getOctreeOctreeDistance)
- [simGeom.getOctreePtcloudDistance](#getOctreePtcloudDistance)
- [simGeom.getOctreeTriangleDistance](#getOctreeTriangleDistance)
- [simGeom.getOctreeSegmentDistance](#getOctreeSegmentDistance)
- [simGeom.getOctreePointDistance](#getOctreePointDistance)

## Point-cloud minimum distance calculation

- [simGeom.getOctreePtcloudDistance](#getOctreePtcloudDistance)
- [simGeom.getMeshPtcloudDistance](#getMeshPtcloudDistance)
- [simGeom.getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance)
- [simGeom.getPtcloudTriangleDistance](#getPtcloudTriangleDistance)
- [simGeom.getPtcloudSegmentDistance](#getPtcloudSegmentDistance)
- [simGeom.getPtcloudPointDistance](#getPtcloudPointDistance)

## Primitives minimum distance calculation

- [simGeom.getBoxBoxDistance](#getBoxBoxDistance)
- [simGeom.getBoxTriangleDistance](#getBoxTriangleDistance)
- [simGeom.getBoxSegmentDistance](#getBoxSegmentDistance)
- [simGeom.getBoxPointDistance](#getBoxPointDistance)
- [simGeom.getTriangleTriangleDistance](#getTriangleTriangleDistance)
- [simGeom.getTriangleSegmentDistance](#getTriangleSegmentDistance)
- [simGeom.getTrianglePointDistance](#getTrianglePointDistance)
- [simGeom.getSegmentSegmentDistance](#getSegmentSegmentDistance)
- [simGeom.getSegmentPointDistance](#getSegmentPointDistance)

## Other API functions

- [simGeom.getTransformedPoints](#getTransformedPoints)

## Commands

### <a name="copyMesh" id="copyMesh"></a> <a name="simGeom.copyMesh" id="simGeom.copyMesh"></a> simGeom.copyMesh

Duplicates a mesh object.

#### Synopsis:

```lua
int meshCopyHandle=simGeom.copyMesh(int meshHandle)
```

```python
int meshCopyHandle=simGeom.copyMesh(int meshHandle)
```

#### Parameters:

- **meshHandle** (int): the mesh object to duplicate.

#### Return values:

- **meshCopyHandle** (int): the copy of the mesh object in case of success.

#### See also:

[createMesh](#createMesh)

### <a name="copyOctree" id="copyOctree"></a> <a name="simGeom.copyOctree" id="simGeom.copyOctree"></a> simGeom.copyOctree

Duplicates an OC-tree object.

#### Synopsis:

```lua
int octreeCopyHandle=simGeom.copyOctree(int octreeHandle)
```

```python
int octreeCopyHandle=simGeom.copyOctree(int octreeHandle)
```

#### Parameters:

- **octreeHandle** (int): the OC-tree object to duplicate.

#### Return values:

- **octreeCopyHandle** (int): the handle of the duplicated OC-tree object.

### <a name="copyPtcloud" id="copyPtcloud"></a> <a name="simGeom.copyPtcloud" id="simGeom.copyPtcloud"></a> simGeom.copyPtcloud

Duplicates a point-cloud object.

#### Synopsis:

```lua
int ptcloudCopyHandle=simGeom.copyPtcloud(int ptcloudHandle)
```

```python
int ptcloudCopyHandle=simGeom.copyPtcloud(int ptcloudHandle)
```

#### Parameters:

- **ptcloudHandle** (int): the point-cloud object to duplicate.

#### Return values:

- **ptcloudCopyHandle** (int): the handle of the duplicated point-cloud object.

### <a name="createMesh" id="createMesh"></a> <a name="simGeom.createMesh" id="simGeom.createMesh"></a> simGeom.createMesh

Creates a mesh object.

#### Synopsis:

```lua
int meshHandle=simGeom.createMesh(float[] vertices,int[] indices,float[3] meshOriginPos=nil,float[4] meshOriginQuat=nil,float triangleEdgeMaxLength=0.3,int maxTrianglesInBoundingBox=8)
```

```python
int meshHandle=simGeom.createMesh(list vertices,list indices,list meshOriginPos=None,list meshOriginQuat=None,float triangleEdgeMaxLength=0.3,int maxTrianglesInBoundingBox=8)
```

#### Parameters:

- **vertices** (float[]): a table with the vertices of the mesh, expressed relative to the world origin.
- **indices** (int[]): a table with the indices of the mesh.
- **meshOriginPos** (float[3], default: nil): the transformation frame position of the mesh. Can be omitted, in which case the frame of the mesh is located at the world origin.
- **meshOriginQuat** (float[4], default: nil): the transformation frame quaternion of the mesh. Can be omitted, in which case the frame of the mesh is oriented as the world reference frame.
- **triangleEdgeMaxLength** (float, default: 0.3): the maximum triangle size used internally for efficient calculations. A smaller size usually results in faster calculations, but larger calculation structures. Can be omitted.
- **maxTrianglesInBoundingBox** (int, default: 8): the maximum number of triangles in leaf bounding boxes, used internally for efficient calculations. A smaller value usually results in faster calculations, but larger calculation structures. Can be omitted.

#### Return values:

- **meshHandle** (int): the handle to the mesh object in case of success.

#### See also:

[destroyMesh](#destroyMesh), [copyMesh](#copyMesh), [createMeshFromSerializationData](#createMeshFromSerializationData)

### <a name="createMeshFromSerializationData" id="createMeshFromSerializationData"></a> <a name="simGeom.createMeshFromSerializationData" id="simGeom.createMeshFromSerializationData"></a> simGeom.createMeshFromSerializationData

Creates a mesh object based on serialization data.

#### Synopsis:

```lua
int meshHandle=simGeom.createMeshFromSerializationData(buffer serializationData)
```

```python
int meshHandle=simGeom.createMeshFromSerializationData(bytes serializationData)
```

#### Parameters:

- **serializationData** (buffer): the serialization data previously generated with [getMeshSerializationData](#getMeshSerializationData).

#### Return values:

- **meshHandle** (int): the handle of the created mesh.

#### See also:

[getMeshSerializationData](#getMeshSerializationData)

### <a name="createOctreeFromColorPoints" id="createOctreeFromColorPoints"></a> <a name="simGeom.createOctreeFromColorPoints" id="simGeom.createOctreeFromColorPoints"></a> simGeom.createOctreeFromColorPoints

Creates an OC-tree object from several points with individual colors and individual user data.

#### Synopsis:

```lua
int octreeHandle=simGeom.createOctreeFromColorPoints(float[] points,float[3] octreeOriginPos=nil,float[4] octreeOriginQuat=nil,float cellS=0.05,int[] rgbData=nil,int[] usrData=nil)
```

```python
int octreeHandle=simGeom.createOctreeFromColorPoints(list points,list octreeOriginPos=None,list octreeOriginQuat=None,float cellS=0.05,list rgbData=None,list usrData=None)
```

#### Parameters:

- **points** (float[]): a table with point data, expressed relative to the world origin.
- **octreeOriginPos** (float[3], default: nil): the transformation frame position of the OC-tree. Can be omitted, in which case the frame of the OC-tree is located at the world origin.
- **octreeOriginQuat** (float[4], default: nil): the transformation frame quaternion of the OC-tree. Can be omitted, in which case the frame orientation of the OC-tree is same as the world origin.
- **cellS** (float, default: 0.05): The desired OC-tree voxel size. Can be omitted.
- **rgbData** (int[], default: nil): the RGB colors (0-255)of the various points (i.e. one color per point). Can be omitted.
- **usrData** (int[], default: nil): the user data of the various points (i.e. one user data per point). Can be omitted.

#### Return values:

- **octreeHandle** (int): the handle of the created OC-tree object.

#### See also:

[createOctreeFromPoints](#createOctreeFromPoints), [createOctreeFromMesh](#createOctreeFromMesh), [createOctreeFromOctree](#createOctreeFromOctree), [destroyOctree](#destroyOctree), [copyOctree](#copyOctree), [createOctreeFromSerializationData](#createOctreeFromSerializationData)

### <a name="createOctreeFromMesh" id="createOctreeFromMesh"></a> <a name="simGeom.createOctreeFromMesh" id="simGeom.createOctreeFromMesh"></a> simGeom.createOctreeFromMesh

Creates an OC-tree object from a mesh object.

#### Synopsis:

```lua
int octreeHandle=simGeom.createOctreeFromMesh(int meshHandle,float[3] meshPos,float[4] meshQuat,float[3] octreeOriginPos=nil,float[4] octreeOriginQuat=nil,float cellS=0.05,int[3] rgbData=nil,int usrData=0)
```

```python
int octreeHandle=simGeom.createOctreeFromMesh(int meshHandle,list meshPos,list meshQuat,list octreeOriginPos=None,list octreeOriginQuat=None,float cellS=0.05,list rgbData=None,int usrData=0)
```

#### Parameters:

- **meshHandle** (int): the mesh object.
- **meshPos** (float[3]): the position of the mesh object.
- **meshQuat** (float[4]): the quaternion of the mesh object.
- **octreeOriginPos** (float[3], default: nil): the transformation frame position of the OC-tree. Can be omitted, in which case the frame of the OC-tree is located at the world origin.
- **octreeOriginQuat** (float[4], default: nil): the transformation frame quaternion of the OC-tree. Can be omitted, in which case the frame orientation of the OC-tree is same as the world origin.
- **cellS** (float, default: 0.05): The desired OC-tree voxel size. Can be omitted.
- **rgbData** (int[3], default: nil): the RGB color (0-255) of the generated voxels. Can be omitted.
- **usrData** (int, default: 0): the user data of the generated voxels. Can be omitted.

#### Return values:

- **octreeHandle** (int): the handle of the created OC-tree object.

#### See also:

[createOctreeFromPoints](#createOctreeFromPoints), [createOctreeFromColorPoints](#createOctreeFromColorPoints), [createOctreeFromOctree](#createOctreeFromOctree), [destroyOctree](#destroyOctree), [copyOctree](#copyOctree), [createOctreeFromSerializationData](#createOctreeFromSerializationData)

### <a name="createOctreeFromOctree" id="createOctreeFromOctree"></a> <a name="simGeom.createOctreeFromOctree" id="simGeom.createOctreeFromOctree"></a> simGeom.createOctreeFromOctree

Creates an OC-tree object from another OC-tree object.

#### Synopsis:

```lua
int newOctreeHandle=simGeom.createOctreeFromOctree(int octreeHandle,float[3] octreepos,float[4] octreeQuat,float[3] newOctreeOriginPos=nil,float[4] newOctreeOriginQuat=nil,float newOctreeCellS=0.05,int[3] rgbData=nil,int usrData=0)
```

```python
int newOctreeHandle=simGeom.createOctreeFromOctree(int octreeHandle,list octreepos,list octreeQuat,list newOctreeOriginPos=None,list newOctreeOriginQuat=None,float newOctreeCellS=0.05,list rgbData=None,int usrData=0)
```

#### Parameters:

- **octreeHandle** (int): the handle to the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree object.
- **octreeQuat** (float[4]): the quaternion of the OC-tree object.
- **newOctreeOriginPos** (float[3], default: nil): the transformation frame position of the new OC-tree. Can be omitted, in which case the frame of the new OC-tree is located at the world origin.
- **newOctreeOriginQuat** (float[4], default: nil): the transformation frame quaternion of the new OC-tree. Can be omitted, in which case the frame orientation of the new OC-tree is same as the world origin.
- **newOctreeCellS** (float, default: 0.05): The desired OC-tree voxel size. Can be omitted.
- **rgbData** (int[3], default: nil): the RGB color (0-255) of the generated voxels. Can be omitted.
- **usrData** (int, default: 0): the user data of the generated voxels. Can be omitted.

#### Return values:

- **newOctreeHandle** (int): the handle of the new OC-tree object.

#### See also:

[createOctreeFromPoints](#createOctreeFromPoints), [createOctreeFromColorPoints](#createOctreeFromColorPoints), [createOctreeFromMesh](#createOctreeFromMesh), [destroyOctree](#destroyOctree), [copyOctree](#copyOctree), [createOctreeFromSerializationData](#createOctreeFromSerializationData)

### <a name="createOctreeFromPoints" id="createOctreeFromPoints"></a> <a name="simGeom.createOctreeFromPoints" id="simGeom.createOctreeFromPoints"></a> simGeom.createOctreeFromPoints

Creates an OC-tree object from several points.

#### Synopsis:

```lua
int octreeHandle=simGeom.createOctreeFromPoints(float[] points,float[3] octreeOriginPos=nil,float[4] octreeOriginQuat=nil,float cellS=0.05,int[] rgbData=nil,int usrData=0)
```

```python
int octreeHandle=simGeom.createOctreeFromPoints(list points,list octreeOriginPos=None,list octreeOriginQuat=None,float cellS=0.05,list rgbData=None,int usrData=0)
```

#### Parameters:

- **points** (float[]): an table with point data, expressed relative to the world origin.
- **octreeOriginPos** (float[3], default: nil): the transformation frame position of the OC-tree. Can be omitted, in which case the frame position of the OC-tree is located at the world origin.
- **octreeOriginQuat** (float[4], default: nil): the transformation frame quaternion of the OC-tree. Can be omitted, in which case the frame orientation of the OC-tree is same as the world origin.
- **cellS** (float, default: 0.05): The desired OC-tree voxel size. Can be omitted.
- **rgbData** (int[], default: nil): the RGB color (0-255) of the generated voxels. Can be omitted.
- **usrData** (int, default: 0): the user data of the generated voxels. Can be omitted.

#### Return values:

- **octreeHandle** (int): the handle of the created OC-tree object.

#### See also:

[createOctreeFromColorPoints](#createOctreeFromColorPoints), [createOctreeFromMesh](#createOctreeFromMesh), [createOctreeFromOctree](#createOctreeFromOctree), [destroyOctree](#destroyOctree), [copyOctree](#copyOctree), [createOctreeFromSerializationData](#createOctreeFromSerializationData)

### <a name="createOctreeFromSerializationData" id="createOctreeFromSerializationData"></a> <a name="simGeom.createOctreeFromSerializationData" id="simGeom.createOctreeFromSerializationData"></a> simGeom.createOctreeFromSerializationData

Creates an OC-tree object based on serialization data.

#### Synopsis:

```lua
int octreeHandle=simGeom.createOctreeFromSerializationData(buffer serializationData)
```

```python
int octreeHandle=simGeom.createOctreeFromSerializationData(bytes serializationData)
```

#### Parameters:

- **serializationData** (buffer): the serialization data previously generated with [getOctreeSerializationData](#getOctreeSerializationData).

#### Return values:

- **octreeHandle** (int): the handle to the created OC-tree object.

#### See also:

[getOctreeSerializationData](#getOctreeSerializationData)

### <a name="createPtcloudFromColorPoints" id="createPtcloudFromColorPoints"></a> <a name="simGeom.createPtcloudFromColorPoints" id="simGeom.createPtcloudFromColorPoints"></a> simGeom.createPtcloudFromColorPoints

Creates a point-cloud object from several points with individual colors.

#### Synopsis:

```lua
int ptcloudHandle=simGeom.createPtcloudFromColorPoints(float[] points,float[3] ptcloudOriginPos=nil,float[4] ptcloudOriginQuat=nil,float cellS=0.05,int maxPointCnt=20,int[] rgbData=nil,float proximityTol=0.005)
```

```python
int ptcloudHandle=simGeom.createPtcloudFromColorPoints(list points,list ptcloudOriginPos=None,list ptcloudOriginQuat=None,float cellS=0.05,int maxPointCnt=20,list rgbData=None,float proximityTol=0.005)
```

#### Parameters:

- **points** (float[]): a table with point data, expressed relative to the world origin.
- **ptcloudOriginPos** (float[3], default: nil): the transformation frame position of the point-cloud. Can be omitted, in which case the frame of the point-cloud is located at the world origin.
- **ptcloudOriginQuat** (float[4], default: nil): the transformation frame quaternion of the point-cloud. Can be omitted, in which case the frame orientation of the point-cloud is same as the world origin.
- **cellS** (float, default: 0.05): The desired voxel size of the underlying OC-tree. Can be omitted.
- **maxPointCnt** (int, default: 20): The maximum amount of points inside of a OC-tree voxel. Can be omitted.
- **rgbData** (int[], default: nil): the RGB color (0-255) of the generated points. Specify one color per point. Can be omitted.
- **proximityTol** (float, default: 0.005): the distance tolerance to other points (i.e. point-point distances below the specified value are not permitted and points will be omitted). Can be omitted.

#### Return values:

- **ptcloudHandle** (int): the handle to the created point-cloud object.

#### See also:

[createPtcloudFromPoints](#createPtcloudFromPoints), [destroyPtcloud](#destroyPtcloud), [copyPtcloud](#copyPtcloud), [createPtcloudFromSerializationData](#createPtcloudFromSerializationData)

### <a name="createPtcloudFromPoints" id="createPtcloudFromPoints"></a> <a name="simGeom.createPtcloudFromPoints" id="simGeom.createPtcloudFromPoints"></a> simGeom.createPtcloudFromPoints

Creates a point-cloud object from several points.

#### Synopsis:

```lua
int ptcloudHandle=simGeom.createPtcloudFromPoints(float[] points,float[3] ptcloudOriginPos=nil,float[4] ptcloudOriginQuat=nil,float cellS=0.05,int maxPointCnt=20,int[3] rgbData=nil,float proximityTol=0.005)
```

```python
int ptcloudHandle=simGeom.createPtcloudFromPoints(list points,list ptcloudOriginPos=None,list ptcloudOriginQuat=None,float cellS=0.05,int maxPointCnt=20,list rgbData=None,float proximityTol=0.005)
```

#### Parameters:

- **points** (float[]): a table with point data, expressed relative to the world origin.
- **ptcloudOriginPos** (float[3], default: nil): the transformation frame position of the point-cloud. Can be omitted, in which case the frame of the point-cloud is located at the world origin.
- **ptcloudOriginQuat** (float[4], default: nil): the transformation frame quaternion of the point-cloud. Can be omitted, in which case the frame orientation of the point-cloud is same as the world origin.
- **cellS** (float, default: 0.05): The desired voxel size of the underlying OC-tree. Can be omitted.
- **maxPointCnt** (int, default: 20): The maximum amount of points inside of a OC-tree voxel. Can be omitted.
- **rgbData** (int[3], default: nil): the RGB color (0-255) of the generated points. Can be omitted.
- **proximityTol** (float, default: 0.005): the distance tolerance to other points (i.e. point-point distances below the specified value are not permitted and points will be omitted). Can be omitted.

#### Return values:

- **ptcloudHandle** (int): the handle to the created point-cloud object.

#### See also:

[createPtcloudFromColorPoints](#createPtcloudFromColorPoints), [destroyPtcloud](#destroyPtcloud), [copyPtcloud](#copyPtcloud), [createPtcloudFromSerializationData](#createPtcloudFromSerializationData)

### <a name="createPtcloudFromSerializationData" id="createPtcloudFromSerializationData"></a> <a name="simGeom.createPtcloudFromSerializationData" id="simGeom.createPtcloudFromSerializationData"></a> simGeom.createPtcloudFromSerializationData

Creates a point-cloud object based on serialization data.

#### Synopsis:

```lua
int ptcloudHandle=simGeom.createPtcloudFromSerializationData(buffer serializationData)
```

```python
int ptcloudHandle=simGeom.createPtcloudFromSerializationData(bytes serializationData)
```

#### Parameters:

- **serializationData** (buffer): the serialization data previously generated with [getPtcloudSerializationData](#getPtcloudSerializationData).

#### Return values:

- **ptcloudHandle** (int): the handle of the created point-cloud.

#### See also:

[getPtcloudSerializationData](#getPtcloudSerializationData)

### <a name="destroyMesh" id="destroyMesh"></a> <a name="simGeom.destroyMesh" id="simGeom.destroyMesh"></a> simGeom.destroyMesh

Destroys a mesh object.

#### Synopsis:

```lua
simGeom.destroyMesh(int meshHandle)
```

```python
simGeom.destroyMesh(int meshHandle)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.

#### Return values:

No return values

#### See also:

[createMesh](#createMesh)

### <a name="destroyOctree" id="destroyOctree"></a> <a name="simGeom.destroyOctree" id="simGeom.destroyOctree"></a> simGeom.destroyOctree

Destroys an OC-tree object.

#### Synopsis:

```lua
simGeom.destroyOctree(int octreeHandle)
```

```python
simGeom.destroyOctree(int octreeHandle)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.

#### Return values:

No return values

#### See also:

[createOctreeFromPoints](#createOctreeFromPoints)

### <a name="destroyPtcloud" id="destroyPtcloud"></a> <a name="simGeom.destroyPtcloud" id="simGeom.destroyPtcloud"></a> simGeom.destroyPtcloud

Destroys a point-cloud object.

#### Synopsis:

```lua
simGeom.destroyPtcloud(int ptcloudHandle)
```

```python
simGeom.destroyPtcloud(int ptcloudHandle)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.

#### Return values:

No return values

#### See also:

[createPtcloudFromPoints](#createPtcloudFromPoints)

### <a name="getBoxBoxDistance" id="getBoxBoxDistance"></a> <a name="simGeom.getBoxBoxDistance" id="simGeom.getBoxBoxDistance"></a> simGeom.getBoxBoxDistance

Computes the minimum distance between two cuboids.

#### Synopsis:

```lua
float distance,float[3] distSegPt1,float[3] distSegPt2=simGeom.getBoxBoxDistance(float[3] box1Pos,float[4] box1Quat,float[3] box1HalfSize,float[3] box2Pos,float[4] box2Quat,float[3] box2HalfSize,bool boxesAreSolid,bool altRoutine=false)
```

```python
float distance,list distSegPt1,list distSegPt2=simGeom.getBoxBoxDistance(list box1Pos,list box1Quat,list box1HalfSize,list box2Pos,list box2Quat,list box2HalfSize,bool boxesAreSolid,bool altRoutine=False)
```

#### Parameters:

- **box1Pos** (float[3]): the position of the first cuboid.
- **box1Quat** (float[4]): the quaternion of the first cuboid.
- **box1HalfSize** (float[3]): the half-size of the first cuboid.
- **box2Pos** (float[3]): the position of the second cuboid.
- **box2Quat** (float[4]): the quaternion of the second cuboid.
- **box2HalfSize** (float[3]): the half-size of the second cuboid.
- **boxesAreSolid** (bool): whether the two cuboids are solid. If true, then cuboid A being fully encompassed by cuboid B will always lead to a distance of zero between the two cuboids.
- **altRoutine** (bool, default: false): if true, an alternative calculation routine will be used. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **distSegPt1** (float[3]): the minimum distance point on the first cuboid.
- **distSegPt2** (float[3]): the minimum distance point on the second cuboid.

#### See also:

[getBoxTriangleDistance](#getBoxTriangleDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getBoxPointDistance](#getBoxPointDistance)

### <a name="getBoxPointDistance" id="getBoxPointDistance"></a> <a name="simGeom.getBoxPointDistance" id="simGeom.getBoxPointDistance"></a> simGeom.getBoxPointDistance

Computes the minimum distance between a cuboid and a point.

#### Synopsis:

```lua
float distance,float[3] boxDistSegPt=simGeom.getBoxPointDistance(float[3] boxPos,float[3] boxQuat,float[3] boxHalfSize,bool boxIsSolid,float[3] point)
```

```python
float distance,list boxDistSegPt=simGeom.getBoxPointDistance(list boxPos,list boxQuat,list boxHalfSize,bool boxIsSolid,list point)
```

#### Parameters:

- **boxPos** (float[3]): the position of the cuboid.
- **boxQuat** (float[3]): the quaternion of the cuboid.
- **boxHalfSize** (float[3]): the half-size of the cuboid.
- **boxIsSolid** (bool): whether the cuboid is solid. If true, then the point being fully encompassed by the cuboid will always lead to a distance of zero.
- **point** (float[3]): position of the point.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **boxDistSegPt** (float[3]): the minimum distance point on the cuboid.

#### See also:

[getMeshPointDistance](#getMeshPointDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxBoxDistance](#getBoxBoxDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getBoxSegmentDistance" id="getBoxSegmentDistance"></a> <a name="simGeom.getBoxSegmentDistance" id="simGeom.getBoxSegmentDistance"></a> simGeom.getBoxSegmentDistance

Computes the minimum distance between a cuboid and a segment.

#### Synopsis:

```lua
float distance,float[3] boxDistSegPt,float[3] segmentSegPt=simGeom.getBoxSegmentDistance(float[3] boxPos,float[3] boxQuat,float[3] boxHalfSize,bool boxIsSolid,float[3] segmentEndPoint,float[3] segmentVector,bool altRoutine=false)
```

```python
float distance,list boxDistSegPt,list segmentSegPt=simGeom.getBoxSegmentDistance(list boxPos,list boxQuat,list boxHalfSize,bool boxIsSolid,list segmentEndPoint,list segmentVector,bool altRoutine=False)
```

#### Parameters:

- **boxPos** (float[3]): the position of the cuboid.
- **boxQuat** (float[3]): the quaternion of the cuboid.
- **boxHalfSize** (float[3]): the half-size of the cuboid.
- **boxIsSolid** (bool): whether the cuboid is solid. If true, then the segment being fully encompassed by the cuboid will always lead to a distance of zero.
- **segmentEndPoint** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **altRoutine** (bool, default: false): if true, an alternative calculation routine will be used. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **boxDistSegPt** (float[3]): the minimum distance point on the cuboid.
- **segmentSegPt** (float[3]): the minimum distance point on the segment.

#### See also:

[getMeshSegmentDistance](#getMeshSegmentDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getBoxBoxDistance](#getBoxBoxDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getBoxPointDistance](#getBoxPointDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getSegmentSegmentDistance](#getSegmentSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getBoxTriangleDistance" id="getBoxTriangleDistance"></a> <a name="simGeom.getBoxTriangleDistance" id="simGeom.getBoxTriangleDistance"></a> simGeom.getBoxTriangleDistance

Computes the minimum distance between a cuboid and a triangle.

#### Synopsis:

```lua
float distance,float[3] boxDistSegPt,float[3] triangleDistSegPt=simGeom.getBoxTriangleDistance(float[3] boxPos,float[3] boxQuat,float[3] boxHalfSize,bool boxIsSolid,float[3] p,float[3] v,float[3] w,bool altRoutine=false)
```

```python
float distance,list boxDistSegPt,list triangleDistSegPt=simGeom.getBoxTriangleDistance(list boxPos,list boxQuat,list boxHalfSize,bool boxIsSolid,list p,list v,list w,bool altRoutine=False)
```

#### Parameters:

- **boxPos** (float[3]): the position of the cuboid.
- **boxQuat** (float[3]): the quaternion of the cuboid.
- **boxHalfSize** (float[3]): the half-size of the cuboid.
- **boxIsSolid** (bool): whether the cuboid is solid. If true, then the triangle being fully encompassed by the cuboid will always lead to a distance of zero.
- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **altRoutine** (bool, default: false): if true, an alternative calculation routine will be used. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **boxDistSegPt** (float[3]): the minimum distance point on the cuboid.
- **triangleDistSegPt** (float[3]): the minimum distance point on the triangle.

#### See also:

[getMeshTriangleDistance](#getMeshTriangleDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getBoxBoxDistance](#getBoxBoxDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getBoxPointDistance](#getBoxPointDistance), [getTriangleTriangleDistance](#getTriangleTriangleDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance)

### <a name="getMeshMeshCollision" id="getMeshMeshCollision"></a> <a name="simGeom.getMeshMeshCollision" id="simGeom.getMeshMeshCollision"></a> simGeom.getMeshMeshCollision

Checks for collision between two mesh objects.

#### Synopsis:

```lua
bool collisionResult,int[2] caching,float[] intersections=simGeom.getMeshMeshCollision(int mesh1Handle,float[3] mesh1Pos,float[4] mesh1Quat,int mesh2Handle,float[3] mesh2Pos,float[4] mesh2Quat,int[2] caching=nil,bool returnIntersections=false)
```

```python
bool collisionResult,list caching,list intersections=simGeom.getMeshMeshCollision(int mesh1Handle,list mesh1Pos,list mesh1Quat,int mesh2Handle,list mesh2Pos,list mesh2Quat,list caching=None,bool returnIntersections=False)
```

#### Parameters:

- **mesh1Handle** (int): the handle of the first mesh object.
- **mesh1Pos** (float[3]): the position of the first mesh object.
- **mesh1Quat** (float[4]): the quaternion of the first mesh object.
- **mesh2Handle** (int): the handle of the second mesh object.
- **mesh2Pos** (float[3]): the position of the second mesh object.
- **mesh2Quat** (float[4]): the quaternion of the second mesh object.
- **caching** (int[2], default: nil): a table with two values from previous collision checking between the same two entities, to speed-up calculations.
- **returnIntersections** (bool, default: false): whether the interference segments should be computed and returned.

#### Return values:

- **collisionResult** (bool): the collision status
- **caching** (int[2]): a table with two values that can be used to speed-up next collision checking between the same two entities (memorizes the colliding triangle in the two meshes).
- **intersections** (float[]): the interference segments.

#### See also:

[getMeshOctreeCollision](#getMeshOctreeCollision), [getMeshTriangleCollision](#getMeshTriangleCollision), [getMeshSegmentCollision](#getMeshSegmentCollision)

### <a name="getMeshMeshDistance" id="getMeshMeshDistance"></a> <a name="simGeom.getMeshMeshDistance" id="simGeom.getMeshMeshDistance"></a> simGeom.getMeshMeshDistance

Computes the minimum distance between two mesh objects.

#### Synopsis:

```lua
float distance,float[3] minDistSegPt1,float[3] minDistSegPt2,int[2] caching=simGeom.getMeshMeshDistance(int mesh1Handle,float[3] mesh1Pos,float[4] mesh1Quat,int mesh2Handle,float[3] mesh2Pos,float[4] mesh2Quat,float thresholdDist=nil,int[2] caching=nil)
```

```python
float distance,list minDistSegPt1,list minDistSegPt2,list caching=simGeom.getMeshMeshDistance(int mesh1Handle,list mesh1Pos,list mesh1Quat,int mesh2Handle,list mesh2Pos,list mesh2Quat,float thresholdDist=None,list caching=None)
```

#### Parameters:

- **mesh1Handle** (int): the handle of the first mesh object.
- **mesh1Pos** (float[3]): the position of the first mesh.
- **mesh1Quat** (float[4]): the quaternion of the first mesh.
- **mesh2Handle** (int): the handle of the second mesh object.
- **mesh2Pos** (float[3]): the position of the second mesh.
- **mesh2Quat** (float[4]): the quaternion of the second mesh.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int[2], default: nil): a table with two values from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **minDistSegPt1** (float[3]): the minimum distance point on the first mesh. nil if the distance is not smaller than the threshold distance.
- **minDistSegPt2** (float[3]): the minimum distance point on the second mesh. nil if the distance is not smaller than the threshold distance.
- **caching** (int[2]): a table with two values that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getMeshPointDistance](#getMeshPointDistance)

### <a name="getMeshOctreeCollision" id="getMeshOctreeCollision"></a> <a name="simGeom.getMeshOctreeCollision" id="simGeom.getMeshOctreeCollision"></a> simGeom.getMeshOctreeCollision

Checks for collision between a mesh object and an OC-tree object.

#### Synopsis:

```lua
bool collisionResult,int[2] caching=simGeom.getMeshOctreeCollision(int meshHandle,float[3] meshPos,float[4] meshQuat,int octreeHandle,float[3] octreePos,float[4] octreeQuat,int[2] caching=nil)
```

```python
bool collisionResult,list caching=simGeom.getMeshOctreeCollision(int meshHandle,list meshPos,list meshQuat,int octreeHandle,list octreePos,list octreeQuat,list caching=None)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **caching** (int[2], default: nil): a table with two values from previous collision checking between the same two entities, to speed-up calculations.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int[2]): a table with two values that can be used to speed-up next collision checking between the same two entities.

#### See also:

[getMeshMeshCollision](#getMeshMeshCollision), [getMeshTriangleCollision](#getMeshTriangleCollision), [getMeshSegmentCollision](#getMeshSegmentCollision), [getOctreeOctreeCollision](#getOctreeOctreeCollision), [getOctreePtcloudCollision](#getOctreePtcloudCollision), [getOctreeTriangleCollision](#getOctreeTriangleCollision), [getOctreeSegmentCollision](#getOctreeSegmentCollision), [getOctreePointCollision](#getOctreePointCollision)

### <a name="getMeshOctreeDistance" id="getMeshOctreeDistance"></a> <a name="simGeom.getMeshOctreeDistance" id="simGeom.getMeshOctreeDistance"></a> simGeom.getMeshOctreeDistance

Computes the minimum distance between a mesh object and an OC-tree object.

#### Synopsis:

```lua
float distance,float[3] meshMinDistPt,float[3] ocMinDistPt,int[2] caching=simGeom.getMeshOctreeDistance(int meshHandle,float[3] meshPos,float[4] meshQuat,int octreeHandle,float[3] octreePos,float[4] octreeQuat,float thresholdDist=nil,int[2] caching=nil)
```

```python
float distance,list meshMinDistPt,list ocMinDistPt,list caching=simGeom.getMeshOctreeDistance(int meshHandle,list meshPos,list meshQuat,int octreeHandle,list octreePos,list octreeQuat,float thresholdDist=None,list caching=None)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int[2], default: nil): a table with two values from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **meshMinDistPt** (float[3]): the minimum distance point on the mesh. nil if the distance is not smaller than the threshold distance.
- **ocMinDistPt** (float[3]): the minimum distance point on the OC-tree. nil if the distance is not smaller than the threshold distance.
- **caching** (int[2]): a table with two values that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshMeshDistance](#getMeshMeshDistance), [getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreeOctreeDistance](#getOctreeOctreeDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getOctreePointDistance](#getOctreePointDistance)

### <a name="getMeshPointDistance" id="getMeshPointDistance"></a> <a name="simGeom.getMeshPointDistance" id="simGeom.getMeshPointDistance"></a> simGeom.getMeshPointDistance

Computes the minimum distance between a mesh object and a point.

#### Synopsis:

```lua
float distance,float[3] meshMinDistPt,int caching=simGeom.getMeshPointDistance(int meshHandle,float[3] meshPos,float[4] meshQuat,float[3] point,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list meshMinDistPt,int caching=simGeom.getMeshPointDistance(int meshHandle,list meshPos,list meshQuat,list point,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **point** (float[3]): the position of the point.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted or -1.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **meshMinDistPt** (float[3]): the minimum distance point on the mesh. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshMeshDistance](#getMeshMeshDistance), [getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxPointDistance](#getBoxPointDistance), [getTrianglePointDistance](#getTrianglePointDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getMeshPtcloudDistance" id="getMeshPtcloudDistance"></a> <a name="simGeom.getMeshPtcloudDistance" id="simGeom.getMeshPtcloudDistance"></a> simGeom.getMeshPtcloudDistance

Computes the minimum distance between a mesh object and a point-cloud object.

#### Synopsis:

```lua
float distance,float[3] meshMinDistPt,float[3] pcMinDistPt,int[2] caching=simGeom.getMeshPtcloudDistance(int meshHandle,float[3] meshPos,float[4] meshQuat,int ptcloudHandle,float[3] pcPos,float[4] pcQuat,float thresholdDist=nil,int[2] caching=nil)
```

```python
float distance,list meshMinDistPt,list pcMinDistPt,list caching=simGeom.getMeshPtcloudDistance(int meshHandle,list meshPos,list meshQuat,int ptcloudHandle,list pcPos,list pcQuat,float thresholdDist=None,list caching=None)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **ptcloudHandle** (int): the handle of the point-cloud object.
- **pcPos** (float[3]): the position of the point-cloud.
- **pcQuat** (float[4]): the quaternion of the point-cloud.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int[2], default: nil): a table with two values from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **meshMinDistPt** (float[3]): the minimum distance point on the mesh. nil if the distance is not smaller than the threshold distance.
- **pcMinDistPt** (float[3]): the minimum distance point on the point-cloud. nil if the distance is not smaller than the threshold distance.
- **caching** (int[2]): a table with two values that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshMeshDistance](#getMeshMeshDistance), [getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getPtcloudPointDistance](#getPtcloudPointDistance)

### <a name="getMeshSegmentCollision" id="getMeshSegmentCollision"></a> <a name="simGeom.getMeshSegmentCollision" id="simGeom.getMeshSegmentCollision"></a> simGeom.getMeshSegmentCollision

Checks for collision between a mesh object and a segment.

#### Synopsis:

```lua
bool collisionResult,int caching,float[] intersections=simGeom.getMeshSegmentCollision(int meshHandle,float[3] meshPos,float[4] meshQuat,float[3] segmentExtremity,float[3] segmentVector,int caching=-1,bool returnIntersections)
```

```python
bool collisionResult,int caching,list intersections=simGeom.getMeshSegmentCollision(int meshHandle,list meshPos,list meshQuat,list segmentExtremity,list segmentVector,int caching=-1,bool returnIntersections)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **segmentExtremity** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **caching** (int, default: -1): a values from previous collision checking between the same two entities, to speed-up collision checking. Can be omitted or set to -1.
- **returnIntersections** (bool): whether the interference segments should be returned. Can be omitted.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a value that can be used to speed-up next collision checking between the same two entities.
- **intersections** (float[]): the interference segments which actually represent one or more segments with zero length.

#### See also:

[getMeshMeshCollision](#getMeshMeshCollision), [getMeshOctreeCollision](#getMeshOctreeCollision), [getMeshTriangleCollision](#getMeshTriangleCollision), [getOctreeSegmentCollision](#getOctreeSegmentCollision)

### <a name="getMeshSegmentDistance" id="getMeshSegmentDistance"></a> <a name="simGeom.getMeshSegmentDistance" id="simGeom.getMeshSegmentDistance"></a> simGeom.getMeshSegmentDistance

Computes the minimum distance between a mesh object and a segment.

#### Synopsis:

```lua
float distance,float[3] meshMinDistPt,float[3] segmentDistSegPt,int caching=simGeom.getMeshSegmentDistance(int meshHandle,float[3] meshPos,float[4] meshQuat,float[3] segmentEndPoint,float[3] segmentVector,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list meshMinDistPt,list segmentDistSegPt,int caching=simGeom.getMeshSegmentDistance(int meshHandle,list meshPos,list meshQuat,list segmentEndPoint,list segmentVector,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **segmentEndPoint** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted or -1.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **meshMinDistPt** (float[3]): the minimum distance point on the mesh. nil if the distance is not smaller than the threshold distance.
- **segmentDistSegPt** (float[3]): the minimum distance point on the segment. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshMeshDistance](#getMeshMeshDistance), [getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getSegmentSegmentDistance](#getSegmentSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getMeshSerializationData" id="getMeshSerializationData"></a> <a name="simGeom.getMeshSerializationData" id="simGeom.getMeshSerializationData"></a> simGeom.getMeshSerializationData

Retrieves the serialization data from a mesh object.

#### Synopsis:

```lua
buffer serializationData=simGeom.getMeshSerializationData(int meshHandle)
```

```python
bytes serializationData=simGeom.getMeshSerializationData(int meshHandle)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.

#### Return values:

- **serializationData** (buffer): the serialization data.

#### See also:

[createMeshFromSerializationData](#createMeshFromSerializationData)

### <a name="getMeshTriangleCollision" id="getMeshTriangleCollision"></a> <a name="simGeom.getMeshTriangleCollision" id="simGeom.getMeshTriangleCollision"></a> simGeom.getMeshTriangleCollision

Checks for collision between a mesh object and a triangle.

#### Synopsis:

```lua
bool collisionResult,int caching,float[] intersections=simGeom.getMeshTriangleCollision(int meshHandle,float[3] meshPos,float[4] meshQuat,float[3] p,float[3] v,float[3] w,int caching=-1,bool returnIntersections)
```

```python
bool collisionResult,int caching,list intersections=simGeom.getMeshTriangleCollision(int meshHandle,list meshPos,list meshQuat,list p,list v,list w,int caching=-1,bool returnIntersections)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **caching** (int, default: -1): a values from previous collision checking between the same two entities, to speed-up collision checking. Can be omitted or set to -1.
- **returnIntersections** (bool): whether the interference segments should be returned. Can be omitted.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a value that can be used to speed-up next collision checking between the same two entities.
- **intersections** (float[]): the interference segments.

#### See also:

[getMeshMeshCollision](#getMeshMeshCollision), [getMeshOctreeCollision](#getMeshOctreeCollision), [getMeshSegmentCollision](#getMeshSegmentCollision), [getOctreeTriangleCollision](#getOctreeTriangleCollision)

### <a name="getMeshTriangleDistance" id="getMeshTriangleDistance"></a> <a name="simGeom.getMeshTriangleDistance" id="simGeom.getMeshTriangleDistance"></a> simGeom.getMeshTriangleDistance

Computes the minimum distance between a mesh object and a triangle.

#### Synopsis:

```lua
float distance,float[3] meshMinDistPt,float[3] triangleMinDistPt,int caching=simGeom.getMeshTriangleDistance(int meshHandle,float[3] meshPos,float[4] meshQuat,float[3] p,float[3] v,float[3] w,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list meshMinDistPt,list triangleMinDistPt,int caching=simGeom.getMeshTriangleDistance(int meshHandle,list meshPos,list meshQuat,list p,list v,list w,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **meshPos** (float[3]): the position of the mesh.
- **meshQuat** (float[4]): the quaternion of the mesh.
- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted or -1.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **meshMinDistPt** (float[3]): the minimum distance point on the mesh. nil if the distance is not smaller than the threshold distance.
- **triangleMinDistPt** (float[3]): the minimum distance point on the triangle. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshMeshDistance](#getMeshMeshDistance), [getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getTriangleTriangleDistance](#getTriangleTriangleDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance)

### <a name="getOctreeOctreeCollision" id="getOctreeOctreeCollision"></a> <a name="simGeom.getOctreeOctreeCollision" id="simGeom.getOctreeOctreeCollision"></a> simGeom.getOctreeOctreeCollision

Checks for collision between two OC-tree objects.

#### Synopsis:

```lua
bool collisionResult,int caching=simGeom.getOctreeOctreeCollision(int oc1Handle,float[3] octree1Pos,float[4] octree1Quat,int oc2Handle,float[3] octree2Pos,float[4] octree2Quat,int[2] caching=nil)
```

```python
bool collisionResult,int caching=simGeom.getOctreeOctreeCollision(int oc1Handle,list octree1Pos,list octree1Quat,int oc2Handle,list octree2Pos,list octree2Quat,list caching=None)
```

#### Parameters:

- **oc1Handle** (int): handle of the first OC-tree object.
- **octree1Pos** (float[3]): the position of the first OC-tree.
- **octree1Pos** (float[3]): the quaternion of the first OC-tree.
- **oc2Handle** (int): handle of the second OC-tree object.
- **octree2Pos** (float[3]): the position of the second OC-tree.
- **octree2Pos** (float[3]): the quaternion of the second OC-tree.
- **caching** (int[2], default: nil): a table with two values from previous collision checking between the same two entities, to speed-up collision checking. Can be omitted.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a table with two values that can be used to speed-up next collision checking between the same two entities.

#### See also:

[getMeshOctreeCollision](#getMeshOctreeCollision), [getOctreePtcloudCollision](#getOctreePtcloudCollision), [getOctreeTriangleCollision](#getOctreeTriangleCollision), [getOctreeSegmentCollision](#getOctreeSegmentCollision), [getOctreePointCollision](#getOctreePointCollision)

### <a name="getOctreeOctreeDistance" id="getOctreeOctreeDistance"></a> <a name="simGeom.getOctreeOctreeDistance" id="simGeom.getOctreeOctreeDistance"></a> simGeom.getOctreeOctreeDistance

Computes the minimum distance between two OC-tree objects.

#### Synopsis:

```lua
float distance,float[3] oc1MinDistPt,float[3] oc2MinDistPt,int[2] caching=simGeom.getOctreeOctreeDistance(int oc1Handle,float[3] octree1Pos,float[4] octree1Quat,int oc2Handle,float[3] octree2Pos,float[4] octree2Quat,float thresholdDist=nil,int[2] caching=nil)
```

```python
float distance,list oc1MinDistPt,list oc2MinDistPt,list caching=simGeom.getOctreeOctreeDistance(int oc1Handle,list octree1Pos,list octree1Quat,int oc2Handle,list octree2Pos,list octree2Quat,float thresholdDist=None,list caching=None)
```

#### Parameters:

- **oc1Handle** (int): the handle of the first OC-tree object.
- **octree1Pos** (float[3]): the position of the first OC-tree.
- **octree1Pos** (float[3]): the quaternion of the first OC-tree.
- **oc2Handle** (int): the handle of the second OC-tree object.
- **octree2Pos** (float[3]): the position of the second OC-tree.
- **octree2Pos** (float[3]): the quaternion of the second OC-tree.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int[2], default: nil): a table with two values from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **oc1MinDistPt** (float[3]): the minimum distance point on the first OC-tree. nil if the distance is not smaller than the threshold distance.
- **oc2MinDistPt** (float[3]): the minimum distance point on the second OC-tree. nil if the distance is not smaller than the threshold distance.
- **caching** (int[2]): a table with two values that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshOctreeDistance](#getMeshOctreeDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getOctreePointDistance](#getOctreePointDistance)

### <a name="getOctreePointCollision" id="getOctreePointCollision"></a> <a name="simGeom.getOctreePointCollision" id="simGeom.getOctreePointCollision"></a> simGeom.getOctreePointCollision

Checks for collision between an OC-tree object and a point.

#### Synopsis:

```lua
bool collisionResult,int caching=simGeom.getOctreePointCollision(int octreeHandle,float[3] octreePos,float[4] octreeQuat,float[3] point,int caching=-1)
```

```python
bool collisionResult,int caching=simGeom.getOctreePointCollision(int octreeHandle,list octreePos,list octreeQuat,list point,int caching=-1)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **point** (float[3]): a point expressed relative to the world reference frame.
- **caching** (int, default: -1): a value from previous collision checking between the same two entities, to speed-up collision checking. Can be omitted or -1.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a value that can be used to speed-up next collision checking between the same two entities.

#### See also:

[getMeshOctreeCollision](#getMeshOctreeCollision), [getOctreeOctreeCollision](#getOctreeOctreeCollision), [getOctreePtcloudCollision](#getOctreePtcloudCollision), [getOctreeTriangleCollision](#getOctreeTriangleCollision), [getOctreeSegmentCollision](#getOctreeSegmentCollision)

### <a name="getOctreePointDistance" id="getOctreePointDistance"></a> <a name="simGeom.getOctreePointDistance" id="simGeom.getOctreePointDistance"></a> simGeom.getOctreePointDistance

Computes the minimum distance between an OC-tree object and a point.

#### Synopsis:

```lua
float distance,float[3] ocMinDistPt,int caching=simGeom.getOctreePointDistance(int octreeHandle,float[3] octreePos,float[4] octreeQuat,float[3] point,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list ocMinDistPt,int caching=simGeom.getOctreePointDistance(int octreeHandle,list octreePos,list octreeQuat,list point,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **point** (float[3]): the position of the point.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted or -1.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **ocMinDistPt** (float[3]): the minimum distance point on the OC-tree. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreeOctreeDistance](#getOctreeOctreeDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxPointDistance](#getBoxPointDistance), [getTrianglePointDistance](#getTrianglePointDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getOctreePtcloudCollision" id="getOctreePtcloudCollision"></a> <a name="simGeom.getOctreePtcloudCollision" id="simGeom.getOctreePtcloudCollision"></a> simGeom.getOctreePtcloudCollision

Checks for collision between am OC-tree object and a point-cloud object.

#### Synopsis:

```lua
bool collisionResult,int caching=simGeom.getOctreePtcloudCollision(int octreeHandle,float[3] octreePos,float[4] octreeQuat,int ptcloudHandle,float[3] ptcloudPos,float[4] ptcloudQuat,int[2] caching=nil)
```

```python
bool collisionResult,int caching=simGeom.getOctreePtcloudCollision(int octreeHandle,list octreePos,list octreeQuat,int ptcloudHandle,list ptcloudPos,list ptcloudQuat,list caching=None)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **ptcloudHandle** (int): the handle of the point-cloud object.
- **ptcloudPos** (float[3]): the position of the point-cloud.
- **ptcloudQuat** (float[4]): the quaternion of the point-cloud.
- **caching** (int[2], default: nil): a table with two values from previous collision checking between the same two entities, to speed-up collision checking..

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a value that can be used to speed-up next collision checking between the same two entities.

#### See also:

[getMeshOctreeCollision](#getMeshOctreeCollision), [getOctreeOctreeCollision](#getOctreeOctreeCollision), [getOctreeTriangleCollision](#getOctreeTriangleCollision), [getOctreeSegmentCollision](#getOctreeSegmentCollision), [getOctreePointCollision](#getOctreePointCollision)

### <a name="getOctreePtcloudDistance" id="getOctreePtcloudDistance"></a> <a name="simGeom.getOctreePtcloudDistance" id="simGeom.getOctreePtcloudDistance"></a> simGeom.getOctreePtcloudDistance

Computes the minimum distance between an OC-tree object and a point-cloud object.

#### Synopsis:

```lua
float distance,float[3] ocMinDistPt,float[3] pcMinDistPt,int[2] caching=simGeom.getOctreePtcloudDistance(int octreeHandle,float[3] octreePos,float[4] octreeQuat,int ptcloudHandle,float[3] pcPos,float[4] pcQuat,float thresholdDist=nil,int[2] caching=nil)
```

```python
float distance,list ocMinDistPt,list pcMinDistPt,list caching=simGeom.getOctreePtcloudDistance(int octreeHandle,list octreePos,list octreeQuat,int ptcloudHandle,list pcPos,list pcQuat,float thresholdDist=None,list caching=None)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **ptcloudHandle** (int): the handle of the point-cloud object.
- **pcPos** (float[3]): the position of the point-cloud.
- **pcQuat** (float[4]): the quaternion of the point-cloud.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int[2], default: nil): a table with two values from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **ocMinDistPt** (float[3]): the minimum distance point on the OC-tree. nil if the distance is not smaller than the threshold distance.
- **pcMinDistPt** (float[3]): the minimum distance point on the point-cloud. nil if the distance is not smaller than the threshold distance.
- **caching** (int[2]): a table with two values that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshPtcloudDistance](#getMeshPtcloudDistance), [getOctreeOctreeDistance](#getOctreeOctreeDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getPtcloudPointDistance](#getPtcloudPointDistance)

### <a name="getOctreeSegmentCollision" id="getOctreeSegmentCollision"></a> <a name="simGeom.getOctreeSegmentCollision" id="simGeom.getOctreeSegmentCollision"></a> simGeom.getOctreeSegmentCollision

Checks for collision between an OC-tree object and a segment.

#### Synopsis:

```lua
bool collisionResult,int caching=simGeom.getOctreeSegmentCollision(int octreeHandle,float[3] octreePos,float[4] octreeQuat,float[3] segmentExtremity,float[3] segmentVector,int caching=-1)
```

```python
bool collisionResult,int caching=simGeom.getOctreeSegmentCollision(int octreeHandle,list octreePos,list octreeQuat,list segmentExtremity,list segmentVector,int caching=-1)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **segmentExtremity** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **caching** (int, default: -1): a value from previous collision checking between the same two entities, to speed-up collision checking. Can be omitted or -1.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a value that can be used to speed-up next collision checking between the same two entities.

#### See also:

[getMeshOctreeCollision](#getMeshOctreeCollision), [getMeshSegmentCollision](#getMeshSegmentCollision), [getOctreeOctreeCollision](#getOctreeOctreeCollision), [getOctreePtcloudCollision](#getOctreePtcloudCollision), [getOctreeTriangleCollision](#getOctreeTriangleCollision), [getOctreePointCollision](#getOctreePointCollision)

### <a name="getOctreeSegmentDistance" id="getOctreeSegmentDistance"></a> <a name="simGeom.getOctreeSegmentDistance" id="simGeom.getOctreeSegmentDistance"></a> simGeom.getOctreeSegmentDistance

Computes the minimum distance between an OC-tree object and a segment.

#### Synopsis:

```lua
float distance,float[3] ocMinDistPt,float[3] segmentDistSegPt,int caching=simGeom.getOctreeSegmentDistance(int octreeHandle,float[3] octreePos,float[4] octreeQuat,float[3] segmentEndPoint,float[3] segmentVector,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list ocMinDistPt,list segmentDistSegPt,int caching=simGeom.getOctreeSegmentDistance(int octreeHandle,list octreePos,list octreeQuat,list segmentEndPoint,list segmentVector,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **segmentEndPoint** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted or -1.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **ocMinDistPt** (float[3]): the minimum distance point on the OC-tree. nil if the distance is not smaller than the threshold distance.
- **segmentDistSegPt** (float[3]): the minimum distance point on the segment. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getOctreeOctreeDistance](#getOctreeOctreeDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getSegmentSegmentDistance](#getSegmentSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getOctreeSerializationData" id="getOctreeSerializationData"></a> <a name="simGeom.getOctreeSerializationData" id="simGeom.getOctreeSerializationData"></a> simGeom.getOctreeSerializationData

Retrieves the serialization data from an OC-tree object.

#### Synopsis:

```lua
buffer serializationData=simGeom.getOctreeSerializationData(int octreeHandle)
```

```python
bytes serializationData=simGeom.getOctreeSerializationData(int octreeHandle)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.

#### Return values:

- **serializationData** (buffer): the serialization data.

#### See also:

[createOctreeFromSerializationData](#createOctreeFromSerializationData)

### <a name="getOctreeTriangleCollision" id="getOctreeTriangleCollision"></a> <a name="simGeom.getOctreeTriangleCollision" id="simGeom.getOctreeTriangleCollision"></a> simGeom.getOctreeTriangleCollision

Checks for collision between an OC-tree object and a triangle.

#### Synopsis:

```lua
bool collisionResult,int caching=simGeom.getOctreeTriangleCollision(int octreeHandle,float[3] octreePos,float[4] octreeQuat,float[3] p,float[3] v,float[3] w,int caching=-1)
```

```python
bool collisionResult,int caching=simGeom.getOctreeTriangleCollision(int octreeHandle,list octreePos,list octreeQuat,list p,list v,list w,int caching=-1)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **caching** (int, default: -1): a value from previous collision checking between the same two entities, to speed-up collision checking. Can be omitted or -1.

#### Return values:

- **collisionResult** (bool): the collision status.
- **caching** (int): a value that can be used to speed-up next collision checking between the same two entities.

#### See also:

[getMeshOctreeCollision](#getMeshOctreeCollision), [getMeshTriangleCollision](#getMeshTriangleCollision), [getOctreeOctreeCollision](#getOctreeOctreeCollision), [getOctreePtcloudCollision](#getOctreePtcloudCollision), [getOctreeSegmentCollision](#getOctreeSegmentCollision), [getOctreePointCollision](#getOctreePointCollision)

### <a name="getOctreeTriangleDistance" id="getOctreeTriangleDistance"></a> <a name="simGeom.getOctreeTriangleDistance" id="simGeom.getOctreeTriangleDistance"></a> simGeom.getOctreeTriangleDistance

Computes the minimum distance between an OC-tree object and a triangle.

#### Synopsis:

```lua
float distance,float[3] ocMinDistPt,float[3] triangleMinDistPt,int caching=simGeom.getOctreeTriangleDistance(int octreeHandle,float[3] octreePos,float[4] octreeQuat,float[3] p,float[3] v,float[3] w,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list ocMinDistPt,list triangleMinDistPt,int caching=simGeom.getOctreeTriangleDistance(int octreeHandle,list octreePos,list octreeQuat,list p,list v,list w,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **octreePos** (float[3]): the position of the OC-tree.
- **octreeQuat** (float[4]): the quaternion of the OC-tree.
- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted or -1.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **ocMinDistPt** (float[3]): the minimum distance point on the OC-tree. nil if the distance is not smaller than the threshold distance.
- **triangleMinDistPt** (float[3]): the minimum distance point on the triangle. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshOctreeDistance](#getMeshOctreeDistance), [getMeshTriangleDistance](#getMeshTriangleDistance), [getOctreeOctreeDistance](#getOctreeOctreeDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getTriangleTriangleDistance](#getTriangleTriangleDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance)

### <a name="getOctreeVoxels" id="getOctreeVoxels"></a> <a name="simGeom.getOctreeVoxels" id="simGeom.getOctreeVoxels"></a> simGeom.getOctreeVoxels

Retrieves voxel data from an OC-tree.

#### Synopsis:

```lua
float[] posData,int[] colorData,int[] userData=simGeom.getOctreeVoxels(int octreeHandle)
```

```python
list posData,list colorData,list userData=simGeom.getOctreeVoxels(int octreeHandle)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.

#### Return values:

- **posData** (float[]): position data: for each voxel, there are 3 position values (XYZ).
- **colorData** (int[]): color data: for each voxel, there are 3 color values (RGB, 0-1).
- **userData** (int[]): user data: for each voxel, there is one user data value.

### <a name="getPtcloudPointDistance" id="getPtcloudPointDistance"></a> <a name="simGeom.getPtcloudPointDistance" id="simGeom.getPtcloudPointDistance"></a> simGeom.getPtcloudPointDistance

Computes the minimum distance between a point-cloud object and a point.

#### Synopsis:

```lua
float distance,float[3] pcMinDistPt,int caching=simGeom.getPtcloudPointDistance(int ptcloudHandle,float[3] pcPos,float[4] pcQuat,float[3] point,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list pcMinDistPt,int caching=simGeom.getPtcloudPointDistance(int ptcloudHandle,list pcPos,list pcQuat,list point,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.
- **pcPos** (float[3]): the position of the point-cloud.
- **pcQuat** (float[4]): the quaternion of the point-cloud.
- **point** (float[3]): the position of the point.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **pcMinDistPt** (float[3]): the minimum distance point on the point-cloud. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getBoxPointDistance](#getBoxPointDistance), [getTrianglePointDistance](#getTrianglePointDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getPtcloudPoints" id="getPtcloudPoints"></a> <a name="simGeom.getPtcloudPoints" id="simGeom.getPtcloudPoints"></a> simGeom.getPtcloudPoints

Retrieves point data from a point-cloud.

#### Synopsis:

```lua
float[] posData,int[] colorData=simGeom.getPtcloudPoints(int ptcloudHandle,float prop=1)
```

```python
list posData,list colorData=simGeom.getPtcloudPoints(int ptcloudHandle,float prop=1)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.
- **prop** (float, default: 1): the proportion of data to retrieve, on a voxel-basis. 1 retrieves all data, 0.25 retrieves 25% of the data.

#### Return values:

- **posData** (float[]): the position data: for each point, there are 3 position values (XYZ).
- **colorData** (int[]): the color data: for each point, there are 3 color values (RGB, 0-1).

### <a name="getPtcloudPtcloudDistance" id="getPtcloudPtcloudDistance"></a> <a name="simGeom.getPtcloudPtcloudDistance" id="simGeom.getPtcloudPtcloudDistance"></a> simGeom.getPtcloudPtcloudDistance

Computes the minimum distance between two point-cloud objects.

#### Synopsis:

```lua
float distance,float[3] pc1MinDistPt,float[3] pc2MinDistPt,int[2] caching=simGeom.getPtcloudPtcloudDistance(int pc1Handle,float[3] pc1pos,float[4] pc1Quat,int pc2Handle,float[3] pc2Pos,float[4] pc2Quat,float distanceThreshold=nil,int[2] caching)
```

```python
float distance,list pc1MinDistPt,list pc2MinDistPt,list caching=simGeom.getPtcloudPtcloudDistance(int pc1Handle,list pc1pos,list pc1Quat,int pc2Handle,list pc2Pos,list pc2Quat,float distanceThreshold=None,list caching)
```

#### Parameters:

- **pc1Handle** (int): the handle of the first point-cloud object.
- **pc1Pos** (float[3]): the position of the first point-cloud.
- **pc1Quat** (float[4]): the quaternion of the first point-cloud.
- **pc2Handle** (int): the handle of the second point-cloud object.
- **pc2Pos** (float[3]): the position of the second point-cloud.
- **pc2Quat** (float[4]): the quaternion of the second point-cloud.
- **distanceThreshold** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int[2]): a table with two values from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **pc1MinDistPt** (float[3]): the minimum distance point on the first point-cloud. nil if the distance is not smaller than the threshold distance.
- **pc2MinDistPt** (float[3]): the minimum distance point on the second point-cloud. nil if the distance is not smaller than the threshold distance.
- **caching** (int[2]): a table with two values that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshPtcloudDistance](#getMeshPtcloudDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getPtcloudPointDistance](#getPtcloudPointDistance)

### <a name="getPtcloudSegmentDistance" id="getPtcloudSegmentDistance"></a> <a name="simGeom.getPtcloudSegmentDistance" id="simGeom.getPtcloudSegmentDistance"></a> simGeom.getPtcloudSegmentDistance

Computes the minimum distance between a point-cloud and a segment.

#### Synopsis:

```lua
float distance,float[3] pcMinDistPt,float[3] segmentDistSegPt,int caching=simGeom.getPtcloudSegmentDistance(int ptcloudHandle,float[3] pcPos,float[4] pcQuat,float[3] segmentEndPoint,float[3] segmentVector,float thresholdDist=nil,int caching=-1)
```

```python
float distance,list pcMinDistPt,list segmentDistSegPt,int caching=simGeom.getPtcloudSegmentDistance(int ptcloudHandle,list pcPos,list pcQuat,list segmentEndPoint,list segmentVector,float thresholdDist=None,int caching=-1)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.
- **pcPos** (float[3]): the position of the point-cloud.
- **pcQuat** (float[4]): the quaternion of the point-cloud.
- **segmentEndPoint** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: -1): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **pcMinDistPt** (float[3]): the minimum distance point on the point-cloud. nil if the distance is not smaller than the threshold distance.
- **segmentDistSegPt** (float[3]): the minimum distance point on the segment. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshPtcloudDistance](#getMeshPtcloudDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getSegmentSegmentDistance](#getSegmentSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getPtcloudSerializationData" id="getPtcloudSerializationData"></a> <a name="simGeom.getPtcloudSerializationData" id="simGeom.getPtcloudSerializationData"></a> simGeom.getPtcloudSerializationData

Retrieves the serialization data from a point-cloud object.

#### Synopsis:

```lua
buffer serializationData=simGeom.getPtcloudSerializationData(int ptcloudHandle)
```

```python
bytes serializationData=simGeom.getPtcloudSerializationData(int ptcloudHandle)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.

#### Return values:

- **serializationData** (buffer): the serialization data.

#### See also:

[createPtcloudFromSerializationData](#createPtcloudFromSerializationData)

### <a name="getPtcloudTriangleDistance" id="getPtcloudTriangleDistance"></a> <a name="simGeom.getPtcloudTriangleDistance" id="simGeom.getPtcloudTriangleDistance"></a> simGeom.getPtcloudTriangleDistance

Computes the minimum distance between a point-cloud object and a triangle.

#### Synopsis:

```lua
float distance,float[3] pcMinDistPt,float[3] triangleMinDistPt,int caching=simGeom.getPtcloudTriangleDistance(int ptcloudHandle,float[3] pcPos,float[4] pcQuat,float[3] p,float[3] v,float[3] w,float thresholdDist=nil,int caching=nil)
```

```python
float distance,list pcMinDistPt,list triangleMinDistPt,int caching=simGeom.getPtcloudTriangleDistance(int ptcloudHandle,list pcPos,list pcQuat,list p,list v,list w,float thresholdDist=None,int caching=None)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.
- **pcPos** (float[3]): the position of the point-cloud.
- **pcQuat** (float[4]): the quaternion of the point-cloud.
- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **thresholdDist** (float, default: nil): the distance threshold. Can be omitted.
- **caching** (int, default: nil): a value from previous distance calculation between the same two entities, to speed-up calculations. Can be omitted.

#### Return values:

- **distance** (float): the minimum distance between the two entities, or if it is not smaller than the threshold distance, the threshold distance.
- **pcMinDistPt** (float[3]): the minimum distance point on the point-cloud. nil if the distance is not smaller than the threshold distance.
- **triangleMinDistPt** (float[3]): the minimum distance point on the triangle. nil if the distance is not smaller than the threshold distance.
- **caching** (int): a value that can be used to speed-up next distance calculation between the same two entities. nil if the distance is not smaller than the threshold distance.

#### See also:

[getMeshPtcloudDistance](#getMeshPtcloudDistance), [getOctreePtcloudDistance](#getOctreePtcloudDistance), [getPtcloudPtcloudDistance](#getPtcloudPtcloudDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getTriangleTriangleDistance](#getTriangleTriangleDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance)

### <a name="getSegmentPointDistance" id="getSegmentPointDistance"></a> <a name="simGeom.getSegmentPointDistance" id="simGeom.getSegmentPointDistance"></a> simGeom.getSegmentPointDistance

Computes the minimum distance between a segment and a point.

#### Synopsis:

```lua
float distance,float[3] segmentDistSegPt=simGeom.getSegmentPointDistance(float[3] segmentEndPoint,float[3] segmentVector,float[3] point)
```

```python
float distance,list segmentDistSegPt=simGeom.getSegmentPointDistance(list segmentEndPoint,list segmentVector,list point)
```

#### Parameters:

- **segmentEndPoint** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.
- **point** (float[3]): position of the point

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **segmentDistSegPt** (float[3]): the minimum distance point on the segment.

#### See also:

[getMeshSegmentDistance](#getMeshSegmentDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getBoxPointDistance](#getBoxPointDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance), [getSegmentSegmentDistance](#getSegmentSegmentDistance)

### <a name="getSegmentSegmentDistance" id="getSegmentSegmentDistance"></a> <a name="simGeom.getSegmentSegmentDistance" id="simGeom.getSegmentSegmentDistance"></a> simGeom.getSegmentSegmentDistance

Computes the minimum distance between two segments.

#### Synopsis:

```lua
float distance,float[3] segment1DistSegPt,float[3] segment2DistSegPt=simGeom.getSegmentSegmentDistance(float[3] segment1EndPoint,float[3] segment1Vector,float[3] segment2EndPoint,float[3] segment2Vector)
```

```python
float distance,list segment1DistSegPt,list segment2DistSegPt=simGeom.getSegmentSegmentDistance(list segment1EndPoint,list segment1Vector,list segment2EndPoint,list segment2Vector)
```

#### Parameters:

- **segment1EndPoint** (float[3]): position of point A on the first segment
- **segment1Vector** (float[3]): vector stretching from point A to point B on the first segment.
- **segment2EndPoint** (float[3]): position of point A on the second segment
- **segment2Vector** (float[3]): vector stretching from point A to point B on the second segment.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **segment1DistSegPt** (float[3]): the minimum distance point on the first segment.
- **segment2DistSegPt** (float[3]): the minimum distance point on the second segment.

#### See also:

[getMeshSegmentDistance](#getMeshSegmentDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getTransformedPoints" id="getTransformedPoints"></a> <a name="simGeom.getTransformedPoints" id="simGeom.getTransformedPoints"></a> simGeom.getTransformedPoints

Transforms points via a transformation matrix, or via a position and a quaternion or Euler angles.

#### Synopsis:

```lua
float[] transformedPoints=simGeom.getTransformedPoints(float[] points,float[12] transformationMatrix)
float[] transformedPoints=simGeom.getTransformedPoints(float[] points,float[3] pos,float[4] quaternion)
float[] transformedPoints=simGeom.getTransformedPoints(float[] points,float[3] pos,float[3] eulerAngles)
```

```python
list transformedPoints=simGeom.getTransformedPoints(list points,list transformationMatrix)
list transformedPoints=simGeom.getTransformedPoints(list points,list pos,list quaternion)
list transformedPoints=simGeom.getTransformedPoints(list points,list pos,list eulerAngles)
```

#### Parameters:

- **points** (float[]): the points to transform.
- **transformationMatrix** (float[12]): a transformation matrix: 12 values (the last row of the 4x4 matrix (0,0,0,1) is not needed). The x-axis of the orientation component is (matrix[1],matrix[5],matrix[9]), the y-axis of the orientation component is (matrix[2],matrix[6],matrix[10]), the z-axis of the orientation component is (matrix[3],matrix[7],matrix[11]), and the translation component is (matrix[4],matrix[8],matrix[12]).
- **pos** (float[3]): a position.
- **quaternion** (float[4]): a quaternion.
- **eulerAngles** (float[3]): Euler angles.

#### Return values:

- **transformedPoints** (float[]): the transformed points.

### <a name="getTrianglePointDistance" id="getTrianglePointDistance"></a> <a name="simGeom.getTrianglePointDistance" id="simGeom.getTrianglePointDistance"></a> simGeom.getTrianglePointDistance

Computes the minimum distance between a triangle and a point.

#### Synopsis:

```lua
float distance,float[3] triangleDistSegPt=simGeom.getTrianglePointDistance(float[3] p,float[3] v,float[3] w,float[3] point)
```

```python
float distance,list triangleDistSegPt=simGeom.getTrianglePointDistance(list p,list v,list w,list point)
```

#### Parameters:

- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **point** (float[3]): position of the point

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **triangleDistSegPt** (float[3]): the minimum distance point on the triangle.

#### See also:

[getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshPointDistance](#getMeshPointDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreePointDistance](#getOctreePointDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudPointDistance](#getPtcloudPointDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getBoxPointDistance](#getBoxPointDistance), [getTriangleTriangleDistance](#getTriangleTriangleDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getTriangleSegmentDistance" id="getTriangleSegmentDistance"></a> <a name="simGeom.getTriangleSegmentDistance" id="simGeom.getTriangleSegmentDistance"></a> simGeom.getTriangleSegmentDistance

Computes the minimum distance between a triangle and a segment.

#### Synopsis:

```lua
float distance,float[3] triangleDistSegPt,float[3] segmentDistSegPt=simGeom.getTriangleSegmentDistance(float[3] p,float[3] v,float[3] w,float[3] segmentEndPoint,float[3] segmentVector)
```

```python
float distance,list triangleDistSegPt,list segmentDistSegPt=simGeom.getTriangleSegmentDistance(list p,list v,list w,list segmentEndPoint,list segmentVector)
```

#### Parameters:

- **p** (float[3]): position of vertex A on the triangle.
- **v** (float[3]): vector stretching from vertex A to vertex B on the triangle.
- **w** (float[3]): vector stretching from vertex A to vertex C on the triangle.
- **segmentEndPoint** (float[3]): position of vertex A on the segment.
- **segmentVector** (float[3]): vector stretching from vertex A to vertex B on the segment.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **triangleDistSegPt** (float[3]): the minimum distance point on the triangle.
- **segmentDistSegPt** (float[3]): the minimum distance point on the segment.

#### See also:

[getMeshTriangleDistance](#getMeshTriangleDistance), [getMeshSegmentDistance](#getMeshSegmentDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getOctreeSegmentDistance](#getOctreeSegmentDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getPtcloudSegmentDistance](#getPtcloudSegmentDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getBoxSegmentDistance](#getBoxSegmentDistance), [getTriangleTriangleDistance](#getTriangleTriangleDistance), [getTrianglePointDistance](#getTrianglePointDistance), [getSegmentSegmentDistance](#getSegmentSegmentDistance), [getSegmentPointDistance](#getSegmentPointDistance)

### <a name="getTriangleTriangleDistance" id="getTriangleTriangleDistance"></a> <a name="simGeom.getTriangleTriangleDistance" id="simGeom.getTriangleTriangleDistance"></a> simGeom.getTriangleTriangleDistance

Computes the minimum distance between two triangles.

#### Synopsis:

```lua
float distance,float[3] distSegPt1,float[3] distSegPt2=simGeom.getTriangleTriangleDistance(float[3] p1,float[3] v1,float[3] w1,float[3] p2,float[3] v2,float[3] w2)
```

```python
float distance,list distSegPt1,list distSegPt2=simGeom.getTriangleTriangleDistance(list p1,list v1,list w1,list p2,list v2,list w2)
```

#### Parameters:

- **p1** (float[3]): position of point A on triangle 1.
- **v1** (float[3]): vector stretching from point A to point B on triangle 1.
- **w1** (float[3]): vector stretching from point A to point C on triangle 1.
- **p2** (float[3]): position of point A on triangle 2.
- **v2** (float[3]): vector stretching from point A to point B on triangle 2.
- **w2** (float[3]): vector stretching from point A to point C on triangle 2.

#### Return values:

- **distance** (float): the minimum distance between the two entities.
- **triangle1DistSegPt** (float[3]): the minimum distance point on the first triangle.
- **triangle2DistSegPt** (float[3]): the minimum distance point on the second triangle.

#### See also:

[getMeshTriangleDistance](#getMeshTriangleDistance), [getOctreeTriangleDistance](#getOctreeTriangleDistance), [getPtcloudTriangleDistance](#getPtcloudTriangleDistance), [getBoxTriangleDistance](#getBoxTriangleDistance), [getTriangleSegmentDistance](#getTriangleSegmentDistance), [getTrianglePointDistance](#getTrianglePointDistance)

### <a name="scaleMesh" id="scaleMesh"></a> <a name="simGeom.scaleMesh" id="simGeom.scaleMesh"></a> simGeom.scaleMesh

Scales a mesh object.

#### Synopsis:

```lua
simGeom.scaleMesh(int meshHandle,float scalingFactor)
```

```python
simGeom.scaleMesh(int meshHandle,float scalingFactor)
```

#### Parameters:

- **meshHandle** (int): the handle of the mesh object.
- **scalingFactor** (float): the scaling factor.

#### Return values:

No return values

### <a name="scaleOctree" id="scaleOctree"></a> <a name="simGeom.scaleOctree" id="simGeom.scaleOctree"></a> simGeom.scaleOctree

Scales an OC-tree object.

#### Synopsis:

```lua
simGeom.scaleOctree(int octreeHandle,float scalingFactor)
```

```python
simGeom.scaleOctree(int octreeHandle,float scalingFactor)
```

#### Parameters:

- **octreeHandle** (int): the handle of the OC-tree object.
- **scalingFactor** (float): the scaling factor.

#### Return values:

No return values

### <a name="scalePtcloud" id="scalePtcloud"></a> <a name="simGeom.scalePtcloud" id="simGeom.scalePtcloud"></a> simGeom.scalePtcloud

Scales a point-cloud object.

#### Synopsis:

```lua
simGeom.scalePtcloud(int ptcloudHandle,float scalingFactor)
```

```python
simGeom.scalePtcloud(int ptcloudHandle,float scalingFactor)
```

#### Parameters:

- **ptcloudHandle** (int): the handle of the point-cloud object.
- **scalingFactor** (float): the scaling factor.

#### Return values:

No return values
