/*
 *  ===========================================================================
 *  Processing Scope Demo
 *  
 *  Scope.pde
 *  
 *  Scope for visualizing data over the computer's serial connection.
 *  
 *  Copyright (C) 2014 JHU BME Builds
 *  ===========================================================================
 */

import processing.serial.*;

Serial myPort;

String myString;

int lf = int('\n');

int nChannels = 3;
int theWidth = 1200;

float[][] dataBuffer = new float[nChannels][theWidth];
float[] curDataMax = new float[nChannels];
float[] curDataMin = new float[nChannels];

float fastBoundDamping = 0.12f;
float boundDamping = 0.08f;

float dynamicRange = 0.7f;

color[] colors = new color[nChannels];
color backgroundColor = color(30);

//float[] filterCoeffs = {1.0f};
// Moving average
int avgMax = 20;
float[] filterCoeffs = new float[avgMax];

void setup ()
{
  size(theWidth, 600);
  // TODO: Kinda broken.
  String portName = Serial.list()[2];
  
  for (int i = 0; i < avgMax; i++)
    filterCoeffs[i] = 1.0f / (float)avgMax;
  
  myPort = new Serial(this, portName, 115200);
  myString = null;
  myPort.clear();
  myString = myPort.readStringUntil(lf);
  myString = null;
  
  for (int i = 0; i < nChannels; i++)
  {
    curDataMin[i] = 0.0f;
    curDataMax[i] = 0.0f;
    
    for (int j = 0; j < theWidth; j++)
    {
      dataBuffer[i][j] = 0.0f;
    }
  }
  
  colors[0] = color(255, 128, 0);
  colors[1] = color(0, 128, 255);
  colors[2] = color(255, 255, 255);
  //colors[3] = color(0, 200, 0);
  //colors[4] = color(0, 0, 200);
}

float[][] firFilt ( float[] ir, float[][] data )
{
  float[][] ret = new float[data.length][data[0].length];
  
  for ( int ch = 0; ch < data.length; ch ++ )
  {
    for ( int n = 0; n < data[0].length; n ++ )
    {
      float denom = 0.0f;
      ret[ch][n] = 0.0f;
      for ( int k = 0; (k < ir.length) && ((n - k) >= 0); k ++ )
      {
        ret[ch][n] += ir[k] * data[ch][n - k];
        denom += abs(ir[k]);
      }
      ret[ch][n] /= denom;
    }
  }
  
  return ret;
}

void shiftData ()
{
  for (int i = 0; i < nChannels; i++)
  {
    for (int j = 0; j < (theWidth - 1); j++)
    {
      dataBuffer[i][j] = dataBuffer[i][j + 1];
    }
  }
}

int getPointY (float datum, float minDat, float maxDat)
{
  if (maxDat == minDat)
    maxDat = minDat + 0.01;
  
  //return (int)(50.0f * datum) + (height/2);
  float datNorm = (datum - minDat) / (maxDat - minDat);
  datNorm = ((1.0f - dynamicRange) / 2.0f) + (dynamicRange * datNorm);
  return (int)((float)height * (1.0f - datNorm));
  //return height * (int)(1.0f - ((datum - minDat) / (maxDat - minDat)));
}

void renderData ()
{
  float[][] dataDisp = firFilt(filterCoeffs, dataBuffer);
  
  // Find the min and max of the data
  
  float[] dataMin = new float[nChannels];
  float[] dataMax = new float[nChannels];
  
  for (int i = 0; i < nChannels; i++)
  {
    dataMin[i] = dataDisp[i][0];
    dataMax[i] = dataDisp[i][0];
    for (int j = 1; j < theWidth; j++)
    {
      if (dataDisp[i][j] > dataMax[i])
        dataMax[i] = dataDisp[i][j];
      else if (dataDisp[i][j] < dataMin[i])
        dataMin[i] = dataDisp[i][j];
    }
    
    if ( dataMin[i] < curDataMin[i] )
      curDataMin[i] += (dataMin[i] - curDataMin[i]) * fastBoundDamping;
    else
      curDataMin[i] += (dataMin[i] - curDataMin[i]) * boundDamping;
    
    if ( dataMax[i] > curDataMax[i] )
      curDataMax[i] += (dataMax[i] - curDataMax[i]) * fastBoundDamping;
    else
      curDataMax[i] += (dataMax[i] - curDataMax[i]) * boundDamping;
  }
  
  //print(dataMax[0]);
  //print(" ");
  //println(dataMin[0]);
  
  background(backgroundColor);
  noFill();
  for (int i = 0; i < nChannels; i++)
  {
    stroke(colors[i]);
    strokeWeight(2);
    beginShape();
    for (int j = 0; j < theWidth; j++)
    {
      vertex(j, getPointY(dataDisp[i][j], curDataMin[i], curDataMax[i]));
    }
    endShape();
    
    //stroke(colors[i]);
    //strokeWeight(3);
    beginShape(POINTS);
    for (int j = 0; j < theWidth; j++)
    {
      vertex(j, getPointY(dataBuffer[i][j], curDataMin[i], curDataMax[i]));
    }
    endShape();
  }
}

void getData ()
{
  while (myPort.available() > 0)
  {
    myString = myPort.readStringUntil(lf);
    if (myString != null)
    {
      //println(myString);
      
      String[] tokens = splitTokens(myString);
      int nTokens = min(tokens.length, nChannels);
      
      shiftData();
      
      float f = 0.0f;
      for (int i = 0; i < nTokens; i++)
      {
        f = 0.0f;
        try
        {
          f = Float.parseFloat(tokens[i]);
          dataBuffer[i][theWidth-1] = f;
        }
        catch (Exception e)
        {
          println("Could not convert one of the read values.");
          dataBuffer[i][theWidth-1] = dataBuffer[i][theWidth-2];
        }
      }
    }
  }
}

void draw ()
{
  
  getData();
  renderData();
  
}
