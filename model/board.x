xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material PDX01_-_Default {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
}

Frame Box01 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh  {
  12;
  -5.000000;0.000000;-0.500000;,
  5.000000;0.000000;-0.500000;,
  -5.000000;0.000000;0.500000;,
  5.000000;0.000000;0.500000;,
  -5.000000;18.000000;-0.500000;,
  -5.000000;18.000000;0.500000;,
  5.000000;18.000000;0.500000;,
  5.000000;0.000000;0.500000;,
  -5.000000;0.000000;0.500000;,
  -5.000000;0.000000;0.500000;,
  -5.000000;18.000000;0.500000;,
  -5.000000;0.000000;-0.500000;;
  6;
  3;0,3,1;,
  3;3,0,2;,
  3;7,5,6;,
  3;5,7,8;,
  3;9,4,10;,
  3;4,9,11;;

  MeshNormals  {
   3;
   0.000000;1.000000;0.000000;,
   0.000000;0.000000;-1.000000;,
   1.000000;0.000000;0.000000;;
   6;
   3;0,0,0;,
   3;0,0,0;,
   3;1,1,1;,
   3;1,1,1;,
   3;2,2,2;,
   3;2,2,2;;
  }

  MeshMaterialList  {
   1;
   6;
   0,
   0,
   0,
   0,
   0,
   0;
   { PDX01_-_Default }
  }

  MeshTextureCoords  {
   12;
   0.000500;0.999498;,
   0.999501;0.999500;,
   0.000500;0.000500;,
   0.999501;0.000502;,
   0.000502;0.000500;,
   0.000500;0.000500;,
   0.999501;0.000500;,
   0.999501;0.999500;,
   0.000500;0.999500;,
   0.999500;0.999500;,
   0.999500;0.000500;,
   0.000502;0.999500;;
  }
 }
}