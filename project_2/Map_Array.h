#ifndef  Map_Array
#define Map_Array


class Map_array{
    private:
    int N;
    int M;
    char G;
    char O;
    double potentialxGO;
    double potentialyGO; 
    double **store;
    double **potentialx_grid;
    double **potentialy_grid;
    int *goalX;
    int *goalY;
    int *obstacleX;
    int *obstacleY;

    public:
    double px;
    double py;
    double K;
    int goal_count;
    int obstacle_count;
    int x;//robot position
    int y;//robot position

    Map_array();
    ~Map_array();
    void print_store();
    void build(int N, int M);
    void map_already_exists(int N, int M);
    void new_GO(char T, int X, int Y);
    double direction(int x, int y);
    void potentialx_to_zero(int N, int M);
    void potentialy_to_zero(int N, int M);
    void clear_arrays();
    void update_k(double k);
    void print_potentialx_to_zero();
    void print_potentialy_to_zero();
    void calculate_potential();
};




