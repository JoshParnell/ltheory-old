/**
 * @file    MarchingCubes.h
 * @author  Thomas Lewiner <thomas.lewiner@polytechnique.org>
 * @author  Math Dept, PUC-Rio
 * @version 0.2
 * @date    12/08/2002
 *
 * @brief   MarchingCubes Algorithm
 */
//________________________________________________


#ifndef _MARCHINGCUBES_H_
#define _MARCHINGCUBES_H_

//_____________________________________________________________________________
// types
/** unsigned char alias */
typedef unsigned char uchar ;
/** signed char alias */
typedef   signed char schar ;
/** isovalue alias */
typedef        float real  ;

//-----------------------------------------------------------------------------
// MCVertex structure
/** \struct MCVertex "MarchingCubes.h" MarchingCubes
 * Position and normal of a MCVertex
 * \brief MCVertex structure
 * \param x X coordinate
 * \param y Y coordinate
 * \param z Z coordinate
 * \param nx X component of the normal
 * \param ny Y component of the normal
 * \param nz Z component of the normal
 */
typedef struct
{
  real  x,  y,  z ;  /**< MCVertex coordinates */
  real nx, ny, nz ;  /**< MCVertex normal */
} MCVertex ;

//-----------------------------------------------------------------------------
// Triangle structure
/** \struct Triangle "MarchingCubes.h" MarchingCubes
 * Indices of the oriented triange vertices
 * \brief triangle structure
 * \param v1 First MCVertex index
 * \param v2 Second MCVertex index
 * \param v3 Third MCVertex index
 */
typedef struct
{
  int v1,v2,v3 ;  /**< Triangle vertices */
} Triangle ;
//_____________________________________________________________________________



//_____________________________________________________________________________
/** Marching Cubes algorithm wrapper */
/** \class MarchingCubes
  * \brief Marching Cubes algorithm.
  */
class MarchingCubes
//-----------------------------------------------------------------------------
{
// Constructors
public :
  /**
   * Main and default constructor
   * \brief constructor
   * \param size_x width  of the grid
   * \param size_y depth  of the grid
   * \param size_z height of the grid
   */
  MarchingCubes ( const int size_x = -1, const int size_y = -1, const int size_z = -1 ) ;
  /** Destructor */
  ~MarchingCubes() ;

//-----------------------------------------------------------------------------
// Accessors
public :
  /** accesses the number of vertices of the generated mesh */
  inline int nverts() const { return _nverts ; }
  /** accesses the number of triangles of the generated mesh */
  inline int ntrigs() const { return _ntrigs ; }
  /** accesses a specific MCVertex of the generated mesh */
  inline MCVertex   * vert( const int i ) const { if( i < 0  || i >= _nverts ) return ( MCVertex *)NULL ; return _vertices  + i ; }
  /** accesses a specific triangle of the generated mesh */
  inline Triangle * trig( const int i ) const { if( i < 0  || i >= _ntrigs ) return (Triangle*)NULL ; return _triangles + i ; }

  /** accesses the MCVertex buffer of the generated mesh */
  inline MCVertex   *vertices () { return _vertices  ; }
  /** accesses the triangle buffer of the generated mesh */
  inline Triangle *triangles() { return _triangles ; }

  /**  accesses the width  of the grid */
  inline int size_x() const { return _size_x ; }
  /**  accesses the depth  of the grid */
  inline int size_y() const { return _size_y ; }
  /**  accesses the height of the grid */
  inline int size_z() const { return _size_z ; }

  /**
   * changes the size of the grid
   * \param size_x width  of the grid
   * \param size_y depth  of the grid
   * \param size_z height of the grid
   */
  inline void set_resolution( const int size_x, const int size_y, const int size_z ) { _size_x = size_x ;  _size_y = size_y ;  _size_z = size_z ; }
  /**
   * selects wether the algorithm will use the enhanced topologically controlled lookup table or the original MarchingCubes
   * \param originalMC true for the original Marching Cubes
   */
  inline void set_method    ( const bool originalMC = false ) { _originalMC = originalMC ; }
  /**
   * selects to use data from another class
   * \param data is the pointer to the external data, allocated as a size_x*size_y*size_z vector running in x first
   */
  inline void set_ext_data  ( real *data )
  { if( !_ext_data ) delete [] _data ;  _ext_data = data != NULL ;  if( _ext_data ) _data = data ; }
  /**
   * selects to allocate data
   */
  inline void set_int_data  () { _ext_data = false ;  _data = NULL ; }

  // Data access
  /**
   * accesses a specific cube of the grid
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline real get_data  ( const int i, const int j, const int k ) const { return _data[ i + j*_size_x + k*_size_x*_size_y] ; }
  /**
   * sets a specific cube of the grid
   * \param val new value for the cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline void  set_data  ( const real val, const int i, const int j, const int k ) { _data[ i + j*_size_x + k*_size_x*_size_y] = val ; }

  // Data initialization
  /** inits temporary structures (must set sizes before call) : the grid and the MCVertex index per cube */
  void init_temps () ;
  /** inits all structures (must set sizes before call) : the temporary structures and the mesh buffers */
  void init_all   () ;
  /** clears temporary structures : the grid and the main */
  void clean_temps() ;
  /** clears all structures : the temporary structures and the mesh buffers */
  void clean_all  () ;

//-----------------------------------------------------------------------------
// Algorithm
public :
  /**
   * Main algorithm : must be called after init_all
   * \param iso isovalue
   */
  void run( real iso = (real)0.0 ) ;

protected :
  /** tesselates one cube */
  void process_cube ()             ;
  /** tests if the components of the tesselation of the cube should be connected by the interior of an ambiguous face */
  bool test_face    ( schar face ) ;
  /** tests if the components of the tesselation of the cube should be connected through the interior of the cube */
  bool test_interior( schar s )    ;


//-----------------------------------------------------------------------------
// Operations
protected :
  /**
   * computes almost all the vertices of the mesh by interpolation along the cubes edges
   * \param iso isovalue
   */
  void compute_intersection_points( real iso ) ;

  /**
   * routine to add a triangle to the mesh
   * \param trig the code for the triangle as a sequence of edges index
   * \param n    the number of triangles to produce
   * \param v12  the index of the interior MCVertex to use, if necessary
   */
  void add_triangle ( const char* trig, char n, int v12 = -1 ) ;

  /** tests and eventually doubles the MCVertex buffer capacity for a new MCVertex insertion */
  void test_MCVertex_addition() ;
  /** adds a MCVertex on the current horizontal edge */
  int add_x_MCVertex() ;
  /** adds a MCVertex on the current longitudinal edge */
  int add_y_MCVertex() ;
  /** adds a MCVertex on the current vertical edge */
  int add_z_MCVertex() ;
  /** adds a MCVertex inside the current cube */
  int add_c_MCVertex() ;

  /**
   * interpolates the horizontal gradient of the implicit function at the lower MCVertex of the specified cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  real get_x_grad( const int i, const int j, const int k ) const ;
  /**
   * interpolates the longitudinal gradient of the implicit function at the lower MCVertex of the specified cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  real get_y_grad( const int i, const int j, const int k ) const ;
  /**
   * interpolates the vertical gradient of the implicit function at the lower MCVertex of the specified cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  real get_z_grad( const int i, const int j, const int k ) const ;

  /**
   * accesses the pre-computed MCVertex index on the lower horizontal edge of a specific cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline int   get_x_vert( const int i, const int j, const int k ) const { return _x_verts[ i + j*_size_x + k*_size_x*_size_y] ; }
  /**
   * accesses the pre-computed MCVertex index on the lower longitudinal edge of a specific cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline int   get_y_vert( const int i, const int j, const int k ) const { return _y_verts[ i + j*_size_x + k*_size_x*_size_y] ; }
  /**
   * accesses the pre-computed MCVertex index on the lower vertical edge of a specific cube
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline int   get_z_vert( const int i, const int j, const int k ) const { return _z_verts[ i + j*_size_x + k*_size_x*_size_y] ; }

  /**
   * sets the pre-computed MCVertex index on the lower horizontal edge of a specific cube
   * \param val the index of the new MCVertex
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline void  set_x_vert( const int val, const int i, const int j, const int k ) { _x_verts[ i + j*_size_x + k*_size_x*_size_y] = val ; }
  /**
   * sets the pre-computed MCVertex index on the lower longitudinal edge of a specific cube
   * \param val the index of the new MCVertex
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline void  set_y_vert( const int val, const int i, const int j, const int k ) { _y_verts[ i + j*_size_x + k*_size_x*_size_y] = val ; }
  /**
   * sets the pre-computed MCVertex index on the lower vertical edge of a specific cube
   * \param val the index of the new MCVertex
   * \param i abscisse of the cube
   * \param j ordinate of the cube
   * \param k height of the cube
   */
  inline void  set_z_vert( const int val, const int i, const int j, const int k ) { _z_verts[ i + j*_size_x + k*_size_x*_size_y] = val ; }

  /** prints cube for debug */
  void print_cube() ;

//-----------------------------------------------------------------------------
// Elements
protected :
  bool      _originalMC ;   /**< selects wether the algorithm will use the enhanced topologically controlled lookup table or the original MarchingCubes */
  bool      _ext_data   ;   /**< selects wether to allocate data or use data from another class */

  int       _size_x     ;  /**< width  of the grid */
  int       _size_y     ;  /**< depth  of the grid */
  int       _size_z     ;  /**< height of the grid */
  real     *_data       ;  /**< implicit function values sampled on the grid */

  int      *_x_verts    ;  /**< pre-computed MCVertex indices on the lower horizontal   edge of each cube */
  int      *_y_verts    ;  /**< pre-computed MCVertex indices on the lower longitudinal edge of each cube */
  int      *_z_verts    ;  /**< pre-computed MCVertex indices on the lower vertical     edge of each cube */

  int       _nverts     ;  /**< number of allocated vertices  in the MCVertex   buffer */
  int       _ntrigs     ;  /**< number of allocated triangles in the triangle buffer */
  int       _Nverts     ;  /**< size of the MCVertex   buffer */
  int       _Ntrigs     ;  /**< size of the triangle buffer */
  MCVertex   *_vertices   ;  /**< MCVertex   buffer */
  Triangle *_triangles  ;  /**< triangle buffer */

  int       _i          ;  /**< abscisse of the active cube */
  int       _j          ;  /**< height of the active cube */
  int       _k          ;  /**< ordinate of the active cube */

  real      _cube[8]    ;  /**< values of the implicit function on the active cube */
  uchar     _lut_entry  ;  /**< cube sign representation in [0..255] */
  uchar     _case       ;  /**< case of the active cube in [0..15] */
  uchar     _config     ;  /**< configuration of the active cube */
  uchar     _subconfig  ;  /**< subconfiguration of the active cube */
};
//_____________________________________________________________________________


#endif // _MARCHINGCUBES_H_

