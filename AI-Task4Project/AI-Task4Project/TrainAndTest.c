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
#include <limits.h>
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

static int classCounter = 0;

static int trainingSetSize = 0;

// performs insertion sort using indexes on K data. (Give data -1 value to ensure that it is inserted properly)
void sortDataViaDistance(int* indexesToRead, double* valueDistance)
{
	int counter = 0;
	double distanceVal[K_NEIGHBOURS];
	int example, example2;
	double tmpIndex = 0, tmpDistance = 0;

	int i;

	for (i = 0; i < K_NEIGHBOURS; i++) // initialise the array. 
	{
		distanceVal[i] = LONG_MAX;
	}

	for (example = 0; example < NUM_SAMPLES; example++)
	{
		if (distanceVal[K_NEIGHBOURS - 1] > valueDistance[example]) 
		{
			distanceVal[K_NEIGHBOURS - 1] = valueDistance[example];
			indexesToRead[K_NEIGHBOURS - 1] = myModelIndex[example];

			// insersion sort here.
			for (example2 = K_NEIGHBOURS - 1; example2 > 0; example2--) 
			{
				if (distanceVal[example2 - 1] > distanceVal[example2]) 
				{
					tmpIndex = indexesToRead[example2 - 1];
					tmpDistance = distanceVal[example2 - 1];
					indexesToRead[example2 - 1] = indexesToRead[example2];
					distanceVal[example2 - 1] = distanceVal[example2];

					distanceVal[example2] = tmpDistance;
					indexesToRead[example2] = tmpIndex;
				}
			}
		}
		// For each sample perform an insertion sort. 
	}
}
	

void getClasses() 
{
	char currentChar = '\0';
	
	int i;
	for (i = 0; i < NUM_TRAINING_SAMPLES; i++) 
	{
		if (currentChar != myModelLabels[i]) 
		{
			currentChar = myModelLabels[i];
			classCounter++;
		}
	}

}


// retrieves distances between data points. 
double getDistance(double* sample1, double* sample2, int numFeatures) 
{
	double diffrence = 0;
	double distance = 0;

	int i;
	for (i = 0; i < numFeatures; i++) 
	{
		diffrence = (sample1[i] - sample2[i]);
		distance += sqrt(diffrence * diffrence);
	}

	return distance;
}


int train( double **trainingSamples, char *trainingLabels, int numSamples, int numFeatures)
{
    int returnval = 1;
	int classValue;
    int sample, feature;
	char classLabels[5];
    
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

	getClasses(); // retrieves the lasses. Will need to check that the data is sorted and if not we will need to sort it. 

    //now you could do whatever you like with the data
    //for example,  you could populate some rules etc.
    //you were given pseudocode in semester 1 to do this
    // you could also normalise the data to remove scaling effects if you want to use something like a MLP or kNN
    //just remember that anything that you want to acess in your predictLabel() function
    //needs to be declared static at the top of this file - as I have done for the "myModel"  and myModelLabels data .
        
    return returnval;
}

char predictLabel(double *sample, int numFeatures)
{
	int predictionInt = 0;
	char prediction = '\0';

	int index = 0;
	double ValueDistance[NUM_TRAINING_SAMPLES];
	int* noOfClassVotes;
	int indexesToCheck[K_NEIGHBOURS];
	int myClass = 0;

	// Do calculations for the distances in kNN. 
	for (index = 0; index < NUM_TRAINING_SAMPLES; index++) 
	{
		ValueDistance[index] = getDistance(sample, &myModel[index],numFeatures);
	}

	// retrieve the K nearest neighbours by finding the top k neighbours with the smallest distance. 
	sortDataViaDistance(indexesToCheck, ValueDistance);

	noOfClassVotes = calloc(classCounter, sizeof(int));

	int j;
	for (j = 0; j < K_NEIGHBOURS; j++)
	{
		myClass = myModelLabels[indexesToCheck[j]];
		myClass -= 97;

		if (ValueDistance[indexesToCheck[j]] == 0.0f) 
		{
			noOfClassVotes[myClass]+= 10;
		}
		else 
		{
			noOfClassVotes[myClass]++;
		}
	}


	// check to see which class has the most votes. 


	// predictions are done via character type hence using integer tricks we can generate our prediction. 
	prediction = (char)(97 + predictionInt);

	// set and return prediction.


       //this is a silly trivial test function
       // obviously you need to replace this with something that uses the model you built in your train() function
       // char prediction = 'c';
    
    return prediction;
}


// We are going to impliment a form of kNN. 
