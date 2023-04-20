/**
 * Projec : gtsp (voyageur de commerce)
 *
 * Date   : 07/04/2023
 * Author : Olivier Grunder, Esteban Becker
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NBR_TOWNS 16

/* Distance matrix */
double dist[NBR_TOWNS][NBR_TOWNS] ;

/* Each edge has a starting and ending node */
int starting_town[NBR_TOWNS] ;
int ending_town[NBR_TOWNS] ;

/* no comment */
int best_solution[NBR_TOWNS] ;
double best_eval=-1.0 ;

int nb_nodes = 0;


/**
 * Berlin52 :
 *  6 towns : Best solution (2315.15): 0 1 2 3 5 4
 * 10 towns : Best solution (2826.50): 0 1 6 2 7 8 9 3 5 4
 */
float coord[NBR_TOWNS][2]=
{
    {565.0,  575.0},
    { 25.0,  185.0},
    {345.0,  750.0},
    {945.0,  685.0},
    {845.0,  655.0},
    {880.0,  660.0},
    {25.0,  230.0},
    {525.0, 1000.0},
    {580.0, 1175.0},
    {650.0, 1130.0},
    {1605.0, 620.0},
    {1220.0, 580.0},
    {1465.0, 200.0},
    {1530.0,  5.0},
    {845.0, 680.0},
    {725.0, 370.0}
    
} ;



/**
 * print a matrix
 */
void print_matrix(double d[NBR_TOWNS][NBR_TOWNS])
{
    int i, j ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("%d:", i) ;
        for (j=0; j<NBR_TOWNS; j++)
        {
            printf ("%6.1f ", d[i][j]) ;
        }
        printf ("\n") ;
    }
}



/**
 * print a solution
 */
void print_solution(int* sol, double eval)
{
    int i ;
    printf ("(%.2f): ", eval) ;
    for (i=0; i<NBR_TOWNS; i++)
        printf ("%d ",sol[i]);
    printf("\n") ;
}




/**
 * evaluation of a solution
 */
double evaluation_solution(int* sol)
{
    double eval=0 ;
    int i ;
    for (i=0; i<NBR_TOWNS-1; i++)
    {
        eval += dist[sol[i]][sol[i+1]] ;
    }
    eval += dist[sol[NBR_TOWNS-1]][sol[0]] ;

    return eval ;

}




/**
 * nearest neighbour solution
 */
double build_nearest_neighbour()
{
    /* solution of the nearest neighbour */
    int i, sol[NBR_TOWNS] ;

    /* evaluation of the solution */
    double eval = 0 ;

    /*initialise solution*/
    for (i=0; i<NBR_TOWNS;i++){
        sol[i] = 0;
    }

    /*find the solution with nearest neighbour*/
    for (i = 1; i<NBR_TOWNS; i++)
    {
        int j, k ;
        double min = -1 ;
        for (j=0; j<NBR_TOWNS; j++)
        {
            for (k=0; k<i; k++)
            {
                if (sol[k] == j)/*If town already in solution*/
                    break ;
            }
            if (k<i)
                /*If town already in solution*/
                continue ;
            if (min<0 || dist[sol[i-1]][j] < min)
            {
                min = dist[sol[i-1]][j] ;
                sol[i] = j ;
            }
        }
    }

    eval = evaluation_solution(sol) ;
    printf("Nearest neighbour ") ;
    print_solution (sol, eval) ;
    
    for (i=0;i<NBR_TOWNS;i++) best_solution[i] = sol[i] ;
    best_eval  = eval ;

    return eval ;
}




/**
 *  Build final solution
 */
void build_solution()
{
    int i, solution[NBR_TOWNS] ;

    int indiceCour = 0;
    int villeCour = 0;

    while (indiceCour < NBR_TOWNS)
    {

        solution[indiceCour] = villeCour ;

        // Test si le cycle est hamiltonien
        for (i = 0; i < indiceCour; i++)
        {
            if (solution[i] == villeCour)
            {
                /* printf ("cycle non hamiltonien\n") ; */
                return;
            }
        }
        // Recherche de la ville suivante
        int trouve = 0;
        int i = 0;
        while ((!trouve) && (i < NBR_TOWNS))
        {
            if (starting_town[i] == villeCour)
            {
                trouve = 1;
                villeCour = ending_town[i];
            }
            i++;
        }
        indiceCour++;
    }

    double eval = evaluation_solution(solution) ;

    if (best_eval<0 || eval < best_eval)
    {
        best_eval = eval ;
        for (i=0; i<NBR_TOWNS; i++)
            best_solution[i] = solution[i] ;
        printf ("New best solution: ") ;
        print_solution (solution, best_eval) ;
    }
    return;
}

/**
 * Eliminate the possible subtours
*/
void subtour_elimination(double d[NBR_TOWNS][NBR_TOWNS], int iteration)
{
    for(int i = 0; i <= iteration; i++)
    {
        int j = 0;
        int found = 1;
        int start = starting_town[i];
        int end = starting_town[i];

        while(j <= iteration && found)
        {
            for (int k = 0; k <= iteration; k++)
            {
                found = 0;
                if(starting_town[k] == end)
                {
                    end = ending_town[k];
                    found = 1;
                    
                }
            }
            if(found)
            {
                d[end][start] = -1;
            }
            j++;
        }
        
    }
}

/**
 *  Little Algorithm
 */
void little_algorithm(double d0[NBR_TOWNS][NBR_TOWNS], int iteration, double eval_node_parent)
{

    nb_nodes++;
    if (iteration == NBR_TOWNS)
    {
        build_solution ();
        return;
    }

    /* Do the modification on a copy of the distance matrix */
    double d[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d, d0, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    int i, j ;

    double eval_node_child = eval_node_parent;

    /* Make sure that there is a 0 in every row*/
    for (i = 0; i < NBR_TOWNS; i++)
    {
        double min = INFINITY;
        for (j = 0; j < NBR_TOWNS; j++)
        {
            if(d[i][j] < min)
            {
                if(d[i][j] != -1)
                    min = d[i][j];
            }
        }
        for (j = 0; j < NBR_TOWNS; j++)
        {
            if(d[i][j] != -1)
                d[i][j] -= min;
        }
        if(min != INFINITY)
            eval_node_child += min;
    }

    /* Make sure that there is a 0 in every column*/
    for (j = 0; j < NBR_TOWNS; j++)
    {
        double min = INFINITY;
        for (i = 0; i < NBR_TOWNS; i++)
        {
            if(d[i][j] < min)
            {
                if(d[i][j] != -1)
                    min = d[i][j];
            }
        }
        for (i = 0; i < NBR_TOWNS; i++)
        {
            if(d[i][j] != -1)
                d[i][j] -= min;
        }
        if(min != INFINITY)
            eval_node_child += min;
    }


    /* Cut : stop the exploration of this node */
    if (best_eval>=0 && eval_node_child >= best_eval)
        return;

    /* Find the zero with the max penalty */
    /* row and column of the zero with the max penalty */
    int izero=-1, jzero=-1 ;
    double max_penalty = -1 ;
    for(i = 0; i < NBR_TOWNS; i++)
    {
        for(j = 0; j < NBR_TOWNS; j++)
        {
            if (d[i][j] == 0)
            {
                double penalty_x = INFINITY , penalty_y = INFINITY ;
                double taken_x = 0 , taken_y = 0 ;
                int k ;
                /* Compute the penalty of the zero */
                for (k = 0; k < NBR_TOWNS; k++)
                {
                    if (k != i && d[k][j] < penalty_x && d[k][j] >= 0){
                        penalty_x = d[k][j] ;
                        taken_x = 1;
                    }
                    if (k != j && d[i][k] < penalty_y && d[i][k] >= 0){
                        penalty_y = d[i][k] ;
                        taken_y = 1;
                    }
                }
                double penalty = penalty_x + penalty_y ;
                
                /* Update the zero with the max penalty */
                if (penalty > max_penalty && (taken_x == 1 || taken_y == 1))
                {
                    max_penalty = penalty ;
                    izero = i ;
                    jzero = j ;
                }else if(taken_x == 0 && taken_y == 0){
                    max_penalty = INFINITY ;
                    izero = i ;
                    jzero = j ;
                }
            }
        }
    }

    if(izero == -1 || jzero == -1)
        return;

    /* Save the starting and ending town of the subtour */
    starting_town[iteration] = izero;
    ending_town[iteration] = jzero;

    /* Do the modification on a copy of the distance matrix */
    double d2[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    /**
     *  Modify the matrix d2 according to the choice of the zero with the max penalty
     */

    /* Set the row and column of the zero with max penalty to -1 */
    for (i = 0; i < NBR_TOWNS; i++)
    {
        d2[izero][i] = -1;
        d2[i][jzero] = -1;
    }

    /*Here we can add a function that avoid the creation of a loop*/
    subtour_elimination(d2, iteration);


    /* Explore left child node according to given choice */
    little_algorithm(d2, iteration + 1, eval_node_child);

    /* Do the modification on a copy of the distance matrix */
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    /**
     *  Modify the dist matrix to explore the other possibility : the non-choice
     *  of the zero with the max penalty
     */

    d2[izero][jzero] = -1;

    /* Explore right child node according to non-choice */
    little_algorithm(d2, iteration, eval_node_child);

}

/**
 *
 */
int main (int argc, char* argv[])
{

    /*Start timer*/
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    best_eval = -1 ;

    /* Print problem informations */
    printf ("Points coordinates:\n") ;
    int i ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("Node %d: x=%f, y=%f\n", i, coord[i][0], coord[i][1]) ;
    }
    printf ("\n") ;


    for (int i=0; i<NBR_TOWNS; i++)
    {
        int j = 0 ;
        while (j < i)
        {
            // Compute the distance between two towns
            dist[i][j] = sqrt (pow(coord[i][0]-coord[j][0],2) + pow(coord[i][1]-coord[j][1],2)) ;
            // Symmetric matrix
            dist[j][i] = dist[i][j] ;
            j++ ;
        }
        dist[i][i] = -1 ; 
    }

    printf ("Distance Matrix:\n") ;
    print_matrix (dist) ;
    printf ("\n") ;

    double nearest_neighbour = build_nearest_neighbour() ;


    int iteration = 0 ;
    double lowerbound = 0.0 ;
    
    little_algorithm(dist, iteration, lowerbound) ;
    
    printf("Best solution:") ;
    print_solution (best_solution, best_eval) ;

    /*End timer*/
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time used: %f\n", cpu_time_used);

    printf("Total number of nodes: %d\n", nb_nodes);

    printf ("Hit RETURN!\n") ;
    getchar() ;

    return 0 ;
}
