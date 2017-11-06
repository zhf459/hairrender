// modified by Shu
// cyCodeBase by Cem Yuksel
// [www.cemyuksel.com]
//-------------------------------------------------------------------------------
//! \file   cyHairFile.h 
//! \author Cem Yuksel
//! 
//! \brief  A class for the HAIR file type
//! 
//-------------------------------------------------------------------------------
//
// Copyright (c) 2016, Cem Yuksel <cem@cemyuksel.com>
// All rights reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal 
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// 
//-------------------------------------------------------------------------------

#ifndef _CY_HAIR_FILE_VEC_H_INCLUDED_
#define _CY_HAIR_FILE_VEC_H_INCLUDED_

//-------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;
//-------------------------------------------------------------------------------
namespace cy {
//-------------------------------------------------------------------------------

#define CY_HAIR_FILE_SEGMENTS_BIT		1
#define CY_HAIR_FILE_POINTS_BIT		2
#define CY_HAIR_FILE_THICKNESS_BIT		4
#define CY_HAIR_FILE_TRANSPARENCY_BIT	8
#define CY_HAIR_FILE_COLORS_BIT		16

#define CY_HAIR_FILE_INFO_SIZE			88

// File read errors
#define CY_HAIR_FILE_ERROR_CANT_OPEN_FILE			-1
#define CY_HAIR_FILE_ERROR_CANT_READ_HEADER		-2
#define	CY_HAIR_FILE_ERROR_WRONG_SIGNATURE			-3
#define	CY_HAIR_FILE_ERROR_READING_SEGMENTS		-4
#define	CY_HAIR_FILE_ERROR_READING_POINTS			-5
#define	CY_HAIR_FILE_ERROR_READING_THICKNESS		-6
#define	CY_HAIR_FILE_ERROR_READING_TRANSPARENCY	-7
#define	CY_HAIR_FILE_ERROR_READING_COLORS			-8

//-------------------------------------------------------------------------------

//! HAIR file class

class HairFileVec
{
public:
	HairFileVec()  { Initialize(); }
	~HairFileVec() { Initialize(); }

	//! Hair file header
	struct Header
	{
		char			signature[4];	//!< This should be "HAIR"
		unsigned int	hair_count;		//!< number of hair strands
		unsigned int	point_count;	//!< total number of points of all strands
		unsigned int	arrays;			//!< bit array of data in the file

		unsigned int	d_segments;		//!< default number of segments of each strand
		float			d_thickness;	//!< default thickness of hair strands
		float			d_transparency;	//!< default transparency of hair strands
		float			d_color[3];		//!< default color of hair strands

		char			info[CY_HAIR_FILE_INFO_SIZE];	//!< information about the file
	};

	//////////////////////////////////////////////////////////////////////////
	//!@name Constant Data Access Methods
	
	const Header& GetHeader() const { return header; }		//!< Use this method to access header data.
	const vector<unsigned short> GetSegmentsArray() const {return segments;}
	const vector<float> GetPointsArray() const { return points;}
	const vector<float> GetThicknessArray() const {return thickness;}
	const vector<float> GetTransparencyArray() const {return transparency;}
	const vector<float> GetColorsArray() const {return colors;}
//	const unsigned short* GetSegmentsArray() const { return segments; }	//!< Returns segments array (segment count for each hair strand).
//	const float* GetPointsArray() const { return points; }				//!< Returns points array (xyz coordinates of each hair point).
//	const float* GetThicknessArray() const { return thickness; }		//!< Returns thickness array (thickness at each hair point}.
//	const float* GetTransparencyArray() const { return transparency; }	//!< Returns transparency array (transparency at each hair point).
//	const float* GetColorsArray() const { return colors; }				//!< Returns colors array (rgb color at each hair point).

	bool SetSegmentsArray(vector<unsigned short> data) 
	{
		if(data.size()==segments.size())
		{
			segments = data;
			return true;
		}
		else
			return false;
	}

	bool SetSegElement(int i,unsigned short val)
	{
		if(i<header.hair_count && i>=0)
		{
			segments[i]=val;
			return true;
		}
		else
			return false;
	}

	bool SetPointsArray(vector<float> data)
	{
		if (data.size()==points.size())
		{
			points = data;
			return true;
		}
		else
			return false;
	}

	bool SetPointElement(int i,float valx,float valy,float valz)
	{
		if(i<header.point_count && i>=0)
		{
			points[3*i]=valx;
			points[3*i+1]=valy;
			points[3*i+2]=valz;
			return true;
		}
		else
			return false;
	}

	bool SetThicknessArray(vector<float> data)
	{
		if(data.size()==thickness.size())
		{
			thickness = data;
			return true;
		}
		else
			 return false;
	}

	bool SetThicknessElement(int i, float val)
	{
		if(i>=0 && i<header.hair_count)
		{
			thickness[i]=val;
			return true;
		}
		else
			return false;
	}

	bool SetTransparencyArray(vector<float> data)
	{
		if(data.size()==transparency.size())
		{
			transparency = data;
			return true;
		}
		else 
			return false;
	}

	bool SetTransparencyElement(int i, float val)
	{
		if(i>=0 && i<header.point_count)
		{
			transparency[i]=val;
			return true;
		}
		else
			 return false;
	}

	bool SetColorsArray(vector<float> data)
	{
		if(data.size()==colors.size())
		{
			colors = data;
			return true;
		}
		else
			return false;
	}

	bool SetColorsElement(int i, float valr,float valg,float valb)
	{
		if(i>=0 && i<header.point_count)
		{
			colors[3*i]=valr;
			colors[3*i+1]=valg;
			colors[3*i+2]=valb;
			return true;
		}
		else
			return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//!@name Data Access Methods

//	unsigned short* GetSegmentsArray() { return segments; }	//!< Returns segments array (segment count for each hair strand).
//	float* GetPointsArray() { return points; }				//!< Returns points array (xyz coordinates of each hair point).
//	float* GetThicknessArray() { return thickness; }		//!< Returns thickness array (thickness at each hair point}.
//	float* GetTransparencyArray() { return transparency; }	//!< Returns transparency array (transparency at each hair point).
//	float* GetColorsArray() { return colors; }				//!< Returns colors array (rgb color at each hair point).


	//////////////////////////////////////////////////////////////////////////
	//!@name Methods for Setting Array Sizes
	
	//! Deletes all arrays and initializes the header data.
	void Initialize()
	{
		if ( segments.size() ) segments.clear();
		if ( points.size() ) points.clear();
		if ( colors.size() ) colors.clear();
		if ( thickness.size() ) thickness.clear();
		if ( transparency.size() ) transparency.clear();
		header.signature[0] = 'H';
		header.signature[1] = 'A';
		header.signature[2] = 'I';
		header.signature[3] = 'R';
		header.hair_count = 0;
		header.point_count = 0;
		header.arrays = 0;	// no arrays
		header.d_segments = 0;
		header.d_thickness = 1.0f;
		header.d_transparency = 0.0f;
		header.d_color[0] = 1.0f;
		header.d_color[1] = 1.0f;
		header.d_color[2] = 1.0f;
		memset( header.info, '\0', CY_HAIR_FILE_INFO_SIZE );
	}

	//! Sets the hair count, re-allocates segments array if necessary.
	void SetHairCount( int count )
	{
		header.hair_count = count;
		if ( segments.size() ) {
			segments.clear();
			segments.resize(header.hair_count);
		}
	}

	// Sets the point count, re-allocates points, thickness, transparency, and colors arrays if necessary.
	void SetPointCount( int count )
	{
		header.point_count = count;
		if ( points.size() ) {
			points.clear();
			points.resize(header.point_count*3);
		}
		if ( thickness.size() ) {
			thickness.clear();
			thickness.resize(header.point_count);
		}
		if ( transparency.size() ) {
			transparency.clear();
			transparency.resize(header.point_count);
		}
		if ( colors.size() ) {
			colors.clear();
			colors.resize(header.point_count*3);
		}
	}

	//! Use this function to allocate/delete arrays.
	//! Before you call this method set hair count and point count.
	//! Note that a valid HAIR file should always have points array.
	void SetArrays( int array_types )
	{
		header.arrays = array_types;
		if ( header.arrays & CY_HAIR_FILE_SEGMENTS_BIT && segments.size()==0 ) segments.resize(header.hair_count);
		if ( ! (header.arrays & CY_HAIR_FILE_SEGMENTS_BIT) && segments.size() ) segments.clear();
		if ( header.arrays & CY_HAIR_FILE_POINTS_BIT && points.size()==0 ) points.resize(header.point_count*3);
		if ( ! (header.arrays & CY_HAIR_FILE_POINTS_BIT) && points.size() ) points.clear();
		if ( header.arrays & CY_HAIR_FILE_THICKNESS_BIT && thickness.size()==0 ) thickness.resize(header.point_count);
		if ( ! (header.arrays & CY_HAIR_FILE_THICKNESS_BIT) && thickness.size() ) thickness.clear();
		if ( header.arrays & CY_HAIR_FILE_TRANSPARENCY_BIT && transparency.size()==0 ) transparency.resize(header.point_count);
		if ( ! (header.arrays & CY_HAIR_FILE_TRANSPARENCY_BIT) && transparency.size() ) transparency.clear();
		if ( header.arrays & CY_HAIR_FILE_COLORS_BIT && colors.size()==0 ) colors.resize(header.point_count*3);
		if ( ! (header.arrays & CY_HAIR_FILE_COLORS_BIT) && colors.size() ) colors.clear();
	}

	//! Sets default number of segments for all hair strands, which is used if segments array does not exist.
	void SetDefaultSegmentCount( int s ) { header.d_segments = s; }

	//! Sets default hair strand thickness, which is used if thickness array does not exist.
	void SetDefaultThickness( float t ) { header.d_thickness = t; }

	//! Sets default hair strand transparency, which is used if transparency array does not exist.
	void SetDefaultTransparency( float t ) { header.d_transparency = t; }

	//! Sets default hair color, which is used if color array does not exist.
	void SetDefaultColor( float r, float g, float b ) { header.d_color[0]=r; header.d_color[1]=g; header.d_color[2]=b; }


	//////////////////////////////////////////////////////////////////////////
	//!@name Load and Save Methods

	//! Loads hair data from the given HAIR file.
	int LoadFromFile( const char *filename )
	{
		Initialize();

		FILE *fp;
		fp = fopen( filename, "rb" );
		if ( fp == NULL ) return CY_HAIR_FILE_ERROR_CANT_OPEN_FILE;

		// read the header
		size_t headread = fread( &header, sizeof(Header), 1, fp );

		#define _CY_FAILED_RETURN(errorno) { Initialize(); fclose( fp ); return errorno; }


		// Check if header is correctly read
		if ( headread < 1 ) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_CANT_READ_HEADER);

		// Check if this is a hair file
		if ( strncmp( header.signature, "HAIR", 4) != 0 && strncmp( header.signature, "hair", 4) != 0) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_WRONG_SIGNATURE);

		// Read segments array
		if ( header.arrays & CY_HAIR_FILE_SEGMENTS_BIT ) {
			unsigned short *data = new unsigned short[ header.hair_count ];
			size_t readcount = fread( data, sizeof(unsigned short), header.hair_count, fp );
			if ( readcount < header.hair_count ) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_READING_SEGMENTS);
			segments.assign(data,data+header.hair_count);
			delete []data;
			
		}

		// Read points array
		if ( header.arrays & CY_HAIR_FILE_POINTS_BIT ) {
			float * data= new float[ header.point_count*3 ];
			size_t readcount = fread( data, sizeof(float), header.point_count*3, fp );
			if ( readcount < header.point_count*3 ) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_READING_POINTS);
			points.assign(data,data+header.point_count*3);
			delete []data;
		}

		// Read thickness array
		if ( header.arrays & CY_HAIR_FILE_THICKNESS_BIT ) {
			float *data = new float[ header.point_count ];
			size_t readcount = fread( data, sizeof(float), header.point_count, fp );
			if ( readcount < header.point_count ) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_READING_THICKNESS);
			thickness.assign(data,data+header.point_count);
			delete []data;
		}

		// Read thickness array
		if ( header.arrays & CY_HAIR_FILE_TRANSPARENCY_BIT ) {
			float *data = new float[ header.point_count ];
			size_t readcount = fread( data, sizeof(float), header.point_count, fp );
			if ( readcount < header.point_count ) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_READING_TRANSPARENCY);
			transparency.assign(data,data+header.point_count);
			delete []data;
		}

		// Read colors array
		if ( header.arrays & CY_HAIR_FILE_COLORS_BIT ) {
			float *data = new float[ header.point_count*3 ];
			size_t readcount = fread(data, sizeof(float), header.point_count*3, fp );
			if ( readcount < header.point_count*3 ) _CY_FAILED_RETURN(CY_HAIR_FILE_ERROR_READING_COLORS);
			colors.assign(data,data+header.point_count*3);
			delete []data;
		}

		fclose( fp );

		return header.hair_count;
	}

	//! Saves hair data to the given HAIR file.
	int SaveToFile( const char *filename ) const
	{
		FILE *fp;
		fp = fopen( filename, "wb" );
		if ( fp == NULL ) return -1;

		// Write header
		fwrite( &header, sizeof(Header), 1, fp );

		// Write arrays
		if ( header.arrays & CY_HAIR_FILE_SEGMENTS_BIT ) fwrite( &segments[0], sizeof(unsigned short), header.hair_count, fp );
		if ( header.arrays & CY_HAIR_FILE_POINTS_BIT ) fwrite( &points[0], sizeof(float), header.point_count*3, fp );
		if ( header.arrays & CY_HAIR_FILE_THICKNESS_BIT ) fwrite( &thickness[0], sizeof(float), header.point_count, fp );
		if ( header.arrays & CY_HAIR_FILE_TRANSPARENCY_BIT ) fwrite( &transparency[0], sizeof(float), header.point_count, fp );
		if ( header.arrays & CY_HAIR_FILE_COLORS_BIT ) fwrite( &colors[0], sizeof(float), header.point_count*3, fp );

		fclose( fp );

		return header.hair_count;
	}


private:
	//////////////////////////////////////////////////////////////////////////
	//!@name Private Variables and Methods

	Header header;
	vector<unsigned short> segments;
	vector<float> points;
	vector<float> thickness;
	vector<float> transparency;
	vector<float> colors;
};

//-------------------------------------------------------------------------------
} // namespace cy
//-------------------------------------------------------------------------------

typedef cy::HairFileVec cyHairFileVec;	//!< HAIR file class

//-------------------------------------------------------------------------------

#endif
