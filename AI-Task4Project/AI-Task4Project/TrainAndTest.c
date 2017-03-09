//
//  TrainAndTest.c
//  MLCoursework
//
//  This is a fairly inefficient implentation that does not use any dynamic memory allocation
// because that wouldnot be safe on the DEWIS marking system
//
//  Created by Jim Smith on 06/02/2017.
//  Copyright © 2017 Jim SmithJim Smith. All rights reserved.
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "TrainAndTest.h"

#define K_NEIGHBOURS 5
#define NUM_CLASSES 10

//declare this array as static but make it available to any function in this file
//in case we want to store the training examples and use them later
static double myModel[NUM_TRAINING_SAMPLES][NUM_FEATURES];

//even if each item in the training set is from a diffferent class we know how many there are.
static char myModelLabels[NUM_TRAINING_SAMPLES];

static int myModelIndex[NUM_TRAINING_SAMPLES];

static int trainingSetSize = 0;

int getClasses() 
{
	int classCounter = 0;
	char className = '\0';

	int i;
	for (i = 0; i < NUM_TRAINING_SAMPLES; i++) 
	{
		if (myModelLabels[i] != className) 
		{
			className = myModelLabels[i];
			classCounter++;
		}
	}

	return classCounter;
}

// performs insertion sort using indexes on K data. (Give data -1 value to ensure that it is inserted properly)
void sortDataViaDistance(int* indexesToRead, double* valueDistance)
{
	int counter = 0;
	int distanceVal[K_NEIGHBOURS];
	int example, example2;
	int i;

	for (i = 0; i < K_NEIGHBOURS; i++) // initialise the array. 
	{
		distanceVal[i] = -1;
	}

	for (example = 0; example < NUM_SAMPLES; example++)
	{
		
		if (distanceVal[K_NEIGHBOURS - 1] > (int)round(valueDistance[K_NEIGHBOURS - 1])) 
		{

		}
		// For each sample perform an insertion sort. 
	}
}
	


// retrieves distances between data points. 
double getDistance(double* sample1, double* sample2) 
{
	double diffrence = 0;
	double distance = 0;

	int i;
	for (i = 0; i < NUM_FEATURES; i++) 
	{
		diffrence = (sample1[i] - sample2[i]);
		distance += sqrt(diffrence * diffrence);
	}

	return distance;
}


int train( double **trainingSamples, char *trainingLabels, int numSamples, int numFeatures)
{
    int returnval = 1;
    int sample, feature;
    
    //clean the model because C leaves whatever is in the memory
    for (sample = 0; sample < NUM_TRAINING_SAMPLES; sample++) 
	{
        for (feature = 0; feature < NUM_FEATURES; feature++) 
		{
            myModel[sample][feature] = 0.0;
        }
    }
    
    //sanity checking
    if ( numFeatures > NUM_FEATURES || numSamples > NUM_TRAINING_SAMPLES) 
	{
        fprintf(stdout,"error: called train with data set larger than spaced allocated to store it");
        returnval = 0;
    }
    
    //this is a silly trivial train()_ function
    // fprintf(stdout,"no ML algorithm implemented yet\n");
    
   
     if(returnval == 1) 
	 {
        //store the labels and the feature values
        trainingSetSize = numSamples;
        int index,feature;

        for (index = 0; index < numSamples; index++)
		{
            myModelLabels[index] = trainingLabels[index];
			myModelIndex[index] = index;

            for (feature = 0; feature < numFeatures; feature++) 
			{
                myModel[index][feature] = trainingSamples[index][feature];
            }
        }

        fprintf(stdout,"data stored locally \n");
    }

    //now you could do whatever you like with the data
    //for example,  you could populate some rules etc.
    //you were given pseudocode in semester 1 to do this
    // you could also normalise the data to remove scaling effects if you want to use something like a MLP or kNN
    //just remember that anything that you want to acess in your predictLabel() function
    //needs to be declared static at the top of this file - as I have done for the "myModel"  and myModelLabels data .
        
    return returnval;
}

char  predictLabel(double *sample, int numFeatures)
{
	int numberOfClasses;
	char prediction = '\0';

	int index = 0;
	double ValueDistance[NUM_TRAINING_SAMPLES];
	int* noOfClassVotes;
	int* indexesToCheck;

	// Do calculations for the distances in kNN. 
	for (index = 0; index < NUM_TRAINING_SAMPLES; index++) 
	{
		ValueDistance[index] = getDistance(sample, &myModel[index]);
	}

	indexesToCheck = calloc(K_NEIGHBOURS, sizeof(int));

	// retrieve the K nearest neighbours by finding the top k neighbours with the smallest distance. 
	sortDataViaDistance(indexesToCheck, ValueDistance);

	noOfClassVotes = calloc(getClasses(), sizeof(int));

	int v;
	for (v = 0; v < numberOfClasses; v++) 
	{
		noOfClassVotes[v] = 0;
	}

	// count the number of nearest neighbours and 

	// set and return prediction.


       //this is a silly trivial test function
       // obviously you need to replace this with something that uses the model you built in your train() function
       // char prediction = 'c';
    
    return prediction;
}


// We are going to impliment a form of kNN. 
