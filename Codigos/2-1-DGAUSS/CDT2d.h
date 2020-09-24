#define Malloc(nSize,nType) (nType *)malloc((unsigned long)(nSize)*(sizeof(nType))) 
#define Malloc2d(nSize,nType) (nType **)malloc2d((unsigned long)(nSize[0]),(unsigned long)(nSize[1]),(sizeof(nType))) 
#define Calloc(nSize,nType) (nType *)calloc((unsigned long)(nSize),(sizeof(nType))) 
#define Calloc2d(nSize,nType) (nType **)calloc2d((unsigned long)(nSize[0]),(unsigned long)(nSize[1]),(sizeof(nType))) 
#define Free(Data) free((char *)Data)
#define Free2d(Data) free2d((char **)Data)

#define DOUBLE_MAX 1.7976931348623158e+308
#define DOUBLE_MIN -1.7976931348623157e+308

// number of intervals for the discretisation of interval [0,pi) 
#define NUMBER_OF_INT 128
// maximum number of directions 
#define MAX_NUMBER_OF_DIR 32
// maximum number of polygons
#define MAX_NUMBER_OF_POLYGONS 1048576
// maximum number of segments
#define MAX_NUMBER_OF_I_SEGMENTS 1048576

typedef struct POLYGON {
	unsigned long numberOfVertices;
	double **V; // list of vertices 0: x-coordinate, 1: y-coordinate (counterclockwise).
	double **H; // support function (fixed direction) 0: minimal value, 1: maximal value.
	double *B;  // width (fixed direction) H[1] - H[0]
	double maxWidth; // maximum of B of all directions: maxWidth = max(B)
	double minWidth; // minimum of B of all directions: minWidth = min(B)
	double wML; // weight for Miles-Lantuejoul edge correction
	double aspectRatio; // aspectRatio = minWidth / maxWidth
	double perimeter; // polygon perimeter
	double area; // polygon area
	double roundness; // polygon roundness --> 4*PI*area / perimeter^2
	double beta; // birthtime
	double tau;  // lifetime
	double time; // time of division => birthtime of the new two cells.
} POLYGON;

typedef struct LINE {
	double alpha; // angle of the normal direction of the line (0 <= alpha < pi).
	double dist; // signed distance of the line from the origin.
	unsigned long index; // index of the normal direction.
} LINE;

typedef struct I_SEGMENT {
	double x0, y0; // x and y coordinate of the starting point.
	double x1, y1; // x and y coordinate of the end point.
	double beta; // birthtime of the segment, which is the division time of the polygon.
	double length; // I_Segment length.
} I_SEGMENT;

typedef struct CDT2d { // stands for Cell Division Tessellation
	POLYGON *Polygon; // list of polygons (cells)
	unsigned long numberOfPolygons; // number of polygons
	I_SEGMENT *I_Segment; // list of segments
	unsigned long numberOfI_Segments; // number of segments 
	unsigned long size; // side length of the quadratic window.
	unsigned long *nSize; // side length of the quadratic window in pixels.
	double *Phi; // distribution function.
	double *PhiCum; // cumulative distribution function.
	double **LinCD; // Linear Contact Dist.(i,j): i-th value of the function with j dilation option. 
	double *SphereCD; // Spherical Contact Distribution.
	/*Gauss modifications*/
	POLYGON *GaussPolygon; // list of polygons to modify
	unsigned long numberOfGaussPolygons; // number of polygons to modify
} CDT2d;

int EmptyCDT2d(CDT2d *, unsigned long size);
void ReadFile(CDT2d *cdt, double *, double *);
void NumberSegmentsCDT2d(unsigned long iVert, unsigned long nVertices, double *, double *, double *, double *);
void InitCDT2d(CDT2d *cdt, unsigned long iVert, unsigned long nVertices, double *, double *);
int FreeCDT2d(CDT2d *);
int SetPhiIso(CDT2d *);
int SetPhiAniso(CDT2d *, double *, double *, unsigned long);
int SetPhiAnisoEllip(CDT2d *, double);
void InitTime(CDT2d *, unsigned long);
int RandomDirection(CDT2d *, double);
LINE RandomLinePolygon(CDT2d *, unsigned long);
LINE RandomLinePolygonDisturbed(CDT2d *, unsigned long, double);
void RandomIntersectionPolygon(CDT2d *, unsigned long, unsigned long, LINE, unsigned long, double);
void RandomIntersectionPolygonDisturbed(CDT2d *, unsigned long, unsigned long, LINE, double, unsigned long, double);
int STIT2dIso(CDT2d *, double, unsigned long, double);
int STIT2dAniso(CDT2d *, double, double *, double *, unsigned long, unsigned long, double);
int STIT2dAnisoDisturbed(CDT2d *, double, double *, double *, unsigned long, double, unsigned long, double);
int STIT2dAnisoEllip(CDT2d *, double, double, unsigned long, double);
int WidthFunction(CDT2d *, POLYGON *);
int PerimeterPolygon(POLYGON *);
int AreaPolygon(POLYGON *);
int RoundnessPolygon(POLYGON *);
double RandomExponential(double);
double RandomElliptic(double);
void StatSTIT(CDT2d *);
void NoBoundary(CDT2d *);
void CopyPolygon(POLYGON *, POLYGON *);

LINE GaussLinePolygon(CDT2d *, unsigned long, double, gsl_rng *);
LINE GaussLinePolygonDisturbed(CDT2d *, unsigned long, double, gsl_rng *, double);
void GaussIntersectionPolygon(CDT2d *, unsigned long, unsigned long, LINE , double , gsl_rng *, unsigned long, double);
void GaussIntersectionPolygonDisturbed(CDT2d *, unsigned long, unsigned long, LINE , double , gsl_rng *, double, unsigned long, double);
int STIT2dGauss(CDT2d *, double, double, unsigned long, double);
int STIT2dGaussDisturbed(CDT2d *, double, double, double, unsigned long, double);
void PlotCDT2d(CDT2d *);

/* memory allocation */
extern void **malloc2d(unsigned long, unsigned long, size_t);
extern void **calloc2d(unsigned long, unsigned long, size_t);
extern void free2d(char **);

extern double RandomUniform(void);


