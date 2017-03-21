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

// TODO - Check how the data is going to be recieved and whether the data will be sourted because if not we will need to run a quicksort
// during the training phase. 

#include <math.h>
#include "TrainAndTest.h"

#define K_NEIGHBOURS 6
#define CLASS_MAX   256
#define LONG_MAX    10000000

//declare this array as static but make it available to any function in this file
//in case we want to store the training examples and use them later
static double myModel[NUM_TRAINING_SAMPLES][NUM_FEATURES];

//even if each item in the training set is from a diffferent class we know how many there are.
static char myModelLabels[NUM_TRAINING_SAMPLES];

static int myModelIndex[NUM_TRAINING_SAMPLES];

// fix for the marking system. 
static int classFrequency[CLASS_MAX]; 

static int trainingSetSize = 0;

// performs insertion sort using indexes on K data.
// BUG - Pointer operations not working. (FIXED)
void sortDataViaDistance(int* indexesToRead, double* valueDistance, int numSamples)
{
	int counter = 0;
	double distanceVal[K_NEIGHBOURS];
	int example, example2;
	double tmpIndex = 0.0f, tmpDistance = 0.0f;
	double ptrValue = 0.0f;

	// initialise the array. 
	int i;
	for (i = 0; i < K_NEIGHBOURS; i++)
	{
		distanceVal[i] = (double)LONG_MAX;
	}

	for (example = 0; example < numSamples; example++)
	{
		ptrValue = valueDistance[example];

		// For each sample perform an insertion sort if nessecery.
		if (distanceVal[K_NEIGHBOURS - 1] > ptrValue) 
		{
			distanceVal[K_NEIGHBOURS - 1] = ptrValue;
			indexesToRead[K_NEIGHBOURS - 1] = example;

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
				else 
				{
					break;
				}
			}
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

char predictLabel(double *sample, int numFeatures)
{
	int predictionInt = 0;
	char prediction = '\0';

	int index = 0;
	double ValueDistance[NUM_TRAINING_SAMPLES];
	int indexesToCheck[K_NEIGHBOURS];
	int myClass = 0;

	// Do calculations for the distances in kNN. 
	for (index = 0; index < NUM_TRAINING_SAMPLES; index++) 
	{
		ValueDistance[index] = getDistance(sample, &myModel[index],numFeatures);
	}

	// retrieve the K nearest neighbours by finding the top k neighbours with the smallest distance. 
	sortDataViaDistance(indexesToCheck, ValueDistance, NUM_TRAINING_SAMPLES);

	// counts the frequency of classification. 
	int k;
	int theClass;
	for (k = 0; k < K_NEIGHBOURS; k++) 
	{
		if (ValueDistance[indexesToCheck[k]] == 0.0f) 
		{
			classFrequency[(int)myModelLabels[indexesToCheck[k]]] += 10;
		}
		else 
		{
			classFrequency[(int)myModelLabels[indexesToCheck[k]]]++;
		}
	}

	for (theClass = 0; theClass < CLASS_MAX; theClass++) 
	{
		if (classFrequency[theClass] > (int)classFrequency[prediction]) 
		{
			prediction = (char)theClass;
		}
	}

	// clean out votes for next run. 
	for (k = 0; k < CLASS_MAX; k++) 
	{
		classFrequency[k] = 0;
	}

	// set and return prediction.
    return prediction;
}
