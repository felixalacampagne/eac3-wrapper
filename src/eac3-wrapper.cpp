//============================================================================
// Name        : eac3-wrapper.cpp
// Author      : Félix à la Campagne
// Version     :
// Copyright   : copyright 2020
// Description : Wrapper program to fix MKVTOMP4 issues with E-AC-3 audio
//============================================================================
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
using namespace std;

bool replace(std::string& str, const std::string& from, const std::string& to) 
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool endsWith(const std::string& str, const std::string& endstr)
{
size_t endlen = endstr.length();
return (str.find(endstr, endlen) != std::string::npos);

}

std::string& quoteArg(std::string& arg)
{
	if(arg.find(" ") != std::string::npos)
	{
		arg.insert(0, "\"");
		arg.append("\"");
	}
	return arg;
}

// Unbelievably there is STILL no way to check whether a file exists in C++ (or in C really)
//bool fileExists(const char *fname)
//{
//struct stat fileAtt;
//bool rc = false;
//	if (stat(fname, &fileAtt) == 0)
//	{
//		rc = true;
//	}
//	return rc;
//}

int main(const int argc, const char **argv) 
{
	//cout << "Eac3to Overriden" << endl; // prints Eac3to Overriden
	string progname = "eac3-wrapper: ";
	string cmdline;
	std::stringstream clstrm;
	string wrapcmd;
	string origeac3;
	string updeac3;
	wrapcmd = argv[0];
	if(endsWith(wrapcmd, ".exe"))
	{
		//cout << "replacing .exe" << endl;
		replace(wrapcmd, ".exe", "_wrpd.exe");
	}
	else
	{
		//cout << "appending _wrpd" << endl;
		wrapcmd += "_wrpd";
	}
	cerr << progname << "wrapping " << wrapcmd << endl;
	clstrm << quoteArg(wrapcmd);

	// Damn! Now need to allow for the .E-AC-3 parameter in any position
	// and it might be an input file instead of an output file.
	// Might still be possible without having to store the args in arrays etc.
	// if it is assumed that only 1 arg will end with .E-AC-3
	//
	// If arg ends with .e-ac-3
	//    create temp .ac3 name
	//    check for .e-ac-3 file presence
	//    	rename to .ac3
	//
	// Do the wrapped command
	//
	// if origeac3 is set
	//    rename .ac3 to .e-ac-3
	// Fork and Shirt!!!!! Bloody QAAC now refuses to process the .ac3 generated
	// by eac3to. This is beyond ridiculous!!!!
	// Looks like I might be able to get away with using .flac instead of .ac3.
	// I guess this won't work if the .E-AC-3 is requried for anything else but
	// as far as I can see it is just an intermediary for the .m4a (AAC) file.
	// NB. eac3to might work if the extension ".ec3" was used, but I guess
	// qaac would still fail when given the .ec3 file.
	// Conversion of .wav to .flac seems extremely slow.... given the
	// file is an intermediary then why not just use .wav?



	for(int i = 1; i < argc; i++)
	{
		string strarg = argv[i];
		if(endsWith(strarg, ".E-AC-3") && updeac3.empty())
		{
			origeac3 = strarg;
			updeac3 = origeac3;
			updeac3 += ".wav";
			strarg = updeac3;

			// Assume an existing eac3 file is for input, so rename it to ac3
			remove(updeac3.c_str());

			// Obviously this will fail if the file is not present, ie. is an output filename
			// It might also fail if paths in the new name are not supported. Have to see what
			// actually happens when program is run!!
			cerr << progname << "rename " << origeac3.c_str() <<
									" to " << updeac3.c_str() << endl;
			rename(origeac3.c_str(), updeac3.c_str());
		}

		// Must quote args containing spaces
		clstrm << " " << quoteArg(strarg);

	}

	cerr << progname << "wrapped command: [" << clstrm.str().c_str() << "]" << endl;
	int rc = std::system(clstrm.str().c_str());

	if( !updeac3.empty())
	{
		remove(origeac3.c_str());
		// This will probably fail, the windows rename command
		// only accepts a filename for the new name.
		int result = rename(updeac3.c_str(),origeac3.c_str());
		if(result != 0)
		{
			cerr << progname << "failed to rename " << updeac3.c_str() <<
					" to " << origeac3.c_str() << endl;
		}
	}
	cerr << progname << " returning " << rc << endl;
	return rc;
}
