#ifndef _3DMODELS_H_
#define _3DMODELS_H_

typedef struct Vertex {
  float x;
  float y; 
  float z;

  u8 r;
  u8 g;
  u8 b;
  
  float nx; 
  float ny; 
  float nz; 
  
  int phi;
  int theta;
  int speed;
} Vertex;

typedef struct Face{
	int a;
	int b;
	int c;
} Face;

typedef struct Mesh{
	char* name;
	Vertex* vertexes;
	int num_of_vertexes;
	Face* faces;
	int num_of_faces;
	Vector3 Position;
	Vector3 BackPosition;
	Vector3 Rotation;
} Mesh;

extern int num_of_cube_vertexes;
extern Vertex CubeV[];
extern int num_of_cube_faces;
extern Face CubeF[];
extern int num_of_work_vertexes;
extern Vertex VWork[];
extern int num_of_tie_vertexes;
extern Vertex TieV[];
extern Face TieF[];
extern int num_of_tie_faces;
extern int num_of_py_vertexes;
extern Vertex PyV[];
extern Face PyF[];
extern int num_of_py_faces;
extern Vertex AsteroidV[];
extern Face AsteroidF[];
extern int num_of_asteroid_vertexes;
extern int num_of_asteroid_faces;
extern Vertex PhotonTV[];
extern Face PhotonTF[];
extern int num_of_photont_vertexes;
extern int num_of_photont_faces;
extern Vertex ShuttleV[];
extern Face ShuttleF[];
extern int num_of_shuttle_vertexes;
extern int num_of_shuttle_faces;
extern Vertex StarbaseV[];
extern Face StarbaseF[];
extern int num_of_starbase_vertexes;
extern int num_of_starbase_faces;

extern struct Mesh Cube;
extern struct Mesh Shuttle;
extern struct Mesh Asteroid;
extern struct Mesh PhotonT;
extern struct Mesh Starbase;
#endif
