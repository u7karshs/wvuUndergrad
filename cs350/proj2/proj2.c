//Barry Martin
//CS310 Project 2
//proj2.c


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int main(int argc, char *argv[])
{
  FILE *myPtr;
  int numNodes;
  int source,dest;
  int debug=0; //turn to 1 to enable debugging
  int *path;

        struct edge
	{
	  int vertexIndex;
	  int vertexWeight;
	  struct edge *edgePtr;
	}edge;

        struct vertex
	{
	  int vertexKey;
	  struct edge *edgePtr;
	  struct vertex *vertexPtr;
	}vertex;

	struct vertex *start;
	start=NULL; //Fixed a seg fault bug in ubuntu?
	struct vertex *currvertex;
	struct edge *curredge;
      
	//make sure we have right number of arguments
	if(argc!=5)
	  {
	    printf("ERROR: Invalid number of arguments.\n");
	    return 0;
	  }
        sscanf(argv[2],"%d",&numNodes);
	sscanf(argv[3],"%d",&source);
	sscanf(argv[4],"%d",&dest);
	
	//Basic error checking
	if(dest==source)
	  {
	    printf("%d:0\n",source);
	    return 0;
	  }
	if(dest<0 || source <0)
	  {
	    printf("ERROR: Negative number specified\n");
	    return 0;
	  }
	if(dest>=numNodes || source>=numNodes)
	  {
	    printf("ERROR: Number(s) specified greater then total number of nodes\n");
	    return 0;
	  }
	
	
	path = (int *) malloc(sizeof(int) * numNodes+1);
	if(debug==1)
	  {
	    printf("Num nodes %d\n",numNodes);
	    printf("Source %d, Dest %d\n",source,dest);
	  }

	
	myPtr=fopen(argv[1],"r");
        if (myPtr==NULL) { return -1; }
        else
	  {
	    int i;
	    for (i=0; i<numNodes; i++)
	      {
		if(debug==1)
		  {
		    printf ("Making vertex %d\n",i);
		  }
		
		struct vertex *v =(struct vertex*)malloc(sizeof(vertex)); //allocate space for vertex 
		
		v->vertexKey=i ; //node vertex id is equal to the current row number; no need for fscanf
		v->edgePtr=NULL;
		v->vertexPtr=NULL;
		
		if (start==NULL)
		  {
		    start=v;
		    currvertex = v;
		  }
		else
		  {
		    currvertex->vertexPtr=v;
		    currvertex=v;
		  }

		int j;
		for (j=0; j<numNodes; j++)
		  {
		    int cw;
		    struct edge *e=malloc(sizeof(edge)); //allocate space for edge
		    if (!feof(myPtr))
		      {
			fscanf(myPtr,"%d",&cw); //read the edge weight into variable cw
		      }
		    else
		      {
			//print error as too few arguments, close the file and exit
			printf("ERROR:  Too few arguments");
			fclose(myPtr);
			return(-1);
		      }

		    if (cw<=0) {}  //if edge weight is 0 or -1, no need to create new edge
		    else
		      {
			e->vertexWeight = cw;
		    	e->vertexIndex = j;
		    	e->edgePtr = NULL;

		      }

		    if (v->edgePtr == NULL)  //first edge for this vertex
		      {

		    	v->edgePtr=e;
		    	curredge = e;
		      }
		    else
		      {
		    	curredge->edgePtr=e;
		    	curredge=e;
		      }


		  }//end for
	      }//end outer for
	  }//end else


        //If debug enabled, print the full graph.
	if(debug==1)
	  {
	    
	    currvertex=start;
	    while (currvertex !=NULL)
	      {
		printf("Vertex: %d\n",currvertex->vertexKey);
		curredge=currvertex->edgePtr;
		while (curredge!=NULL)
		  {
		    printf("%d ",curredge->vertexIndex);
		    printf(" %d\n",curredge->vertexWeight);
		    curredge=curredge->edgePtr;
		  }
		currvertex=currvertex->vertexPtr;
	      }
	  }
       
	//Begin Shortest Path 
       	int v=start->vertexKey;
	currvertex=start;
	int numVisited=0;
	int distance=0;
	while(currvertex->vertexKey!=source)
	  {
	    currvertex=currvertex->vertexPtr;
	  }
	path[0]=source;
	
	
      	while(numVisited<=numNodes)
	  {
	    int i;
	    int min=INT_MAX;
	    
	    if (currvertex->vertexKey!=v) //move to the new vertex
	      {
		currvertex=start; //we might need to backtrack, so start from begining
		while (currvertex->vertexKey!=v)
		  {
		    currvertex=currvertex->vertexPtr;
		  }
		path[numVisited]=currvertex->vertexKey;
		//printf("-%d",currvertex->vertexKey);
	      }
	    
	     if(currvertex->vertexKey==dest) //are we at our destination now?
	      {
		//	printf(":%d\n",distance);
		break;
	      }
	   
	    curredge=currvertex->edgePtr;
	    while(curredge!=NULL) //find min distance
	      {
		
		int weight=curredge->vertexWeight;
		if (weight!=0 && weight<min)
		  {
		    min=weight;
		    v=curredge->vertexIndex;
		  }
		curredge=curredge->edgePtr;
	      }
	    distance+=min;
	    numVisited++;
	  }//end outer while
	
	if(numVisited>=numNodes)
	  printf("NO PATH\n");
	else //print path and distance
	  {
	    printf("%d",source);
	    int i=1;
	    while(path[i]!=dest)
	      {
		printf("-%d",path[i]);
		i++;
	      }
	    printf("-%d:%d\n",path[i],distance);
	  }
	
}
