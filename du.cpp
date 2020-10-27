/*
Project:	du.cpp
Author:		Priscilla Peron
Purpose:	To show disk usage (based on the UNIX command).
			Some pieces of code were extracted and/or inspired in Garth Santor's
			classes during the 2nd term of CPA program at Fanshawe during Winter 2020.
*/
//Make libraries available for the project
#include<locale>
#include<iostream>
#include<fstream>
#include<filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <functional>
#include <iomanip>

using namespace std::filesystem;

// Define a structure to be used
// Similar code used in Garth Santor's classes
class DiskUsage {
public:
	std::string name;
	double space = 0;
	char character = ' ';
};

// Declare the vectors of DiskUsage structure
std::vector<DiskUsage> vector1;
std::vector<std::pair<std::string, uintmax_t>>folders;
std::string myString = "1.0.0";
uintmax_t blockSizeDefined = 4096;

//Predicate function to sort data by file_size
// Similar code used in Garth Santor's classes
bool bySizeF(DiskUsage const& lhs, DiskUsage const& rhs) {
	return lhs.space < rhs.space;
}

//Predicate function to sort data by file_name
// Similar code used in Garth Santor's classes
bool byNameF(DiskUsage const& lhs, DiskUsage const& rhs) {
	return lhs.name < rhs.name;
}

//Predicate function to sort data by file_size
// Similar code used in Garth Santor's classes
bool bySizeP(const std::pair<std::string, uintmax_t>& a, const std::pair<std::string, uintmax_t>& b) {
	return a.second < b.second;
}

//Predicate function to sort data by file_name
// Similar code used in Garth Santor's classes
bool byNameP(const std::pair<std::string, uintmax_t>& a, const std::pair<std::string, uintmax_t>& b) {
	//verificar set 
	return a.first.size() < b.first.size();
}

/**
function: sizeToCluster()
Purpose: calculate the size of the cluster in the recursive function recursive_scan
*/
uintmax_t sizeToCluster(uintmax_t size) {
	return (uintmax_t)ceil((double)size / blockSizeDefined);
}

/**
function: recursive_scan()
Purpose: recursively scan the current directory and all subfolders
*/
// Similar code used in Garth Santor's classes
uintmax_t recursive_scan(std::vector<std::string>& path) {
	uintmax_t folder = 0;
	uintmax_t current = 0;
	uintmax_t total = 0;

	for (unsigned int i = 0; i < path.size(); i++) {
		total = 0;
		directory_iterator d(path[i]);	// first entry of folder 'f'
		directory_iterator e;

		while (d != e) {
			folder = 0;

			if (!is_directory(d->path())) {
				current = sizeToCluster(file_size(d->path()));
				total += current;
			}

			//Iterate path and add all files/folders size. 
			else {
				for (auto f : recursive_directory_iterator(d->path().string(), directory_options::skip_permission_denied)) {
					folder += sizeToCluster(file_size(f.path().string()));

				}
				total += folder;
				folders.push_back(make_pair(d->path().string(), folder));

			}
			++d;
		}
		folders.push_back(make_pair(path[i], total));

	}
	return total;
}

/**
function: getargv()
Purpose: checks for -, -- or . on argv to know what each one is doing
*/
void getargv(std::string arg, std::vector<std::string>& f) {
	size_t length1 = arg.length() - 1;
	for (size_t i = 0; i < length1; i++) {
		char ch;
		std::string word;
		if (arg[i] == '-' && arg[i + 1] == '-') {
			f.push_back(arg.substr(2, arg.length()));
			break;
		}
		else if (arg[i] == '-' && arg[i + 1] != '-') {
			ch = arg[i + 1];
			word.push_back(ch);
			f.push_back(word);
		}
		else if (arg[i] != '-' && arg[i + 1] != '-') {
			if (i == 0) {
				f.push_back(arg);
				break;
			}
			else {
				ch = arg[i + 1];
				word.push_back(ch);
				f.push_back(word);
			}
		}
	}
}

/**
Function: main()
*/
// Parts of code extracted from Garth Santor's classes
int main(int argc, char* argv[]) {


	if (argc == 1) {
		std::vector<std::string> current;
		current.push_back(".");
		try {
			recursive_scan(current);
		}
		catch (filesystem_error e) {
			std::cerr << "Error: " << e.what() << std::endl;
			EXIT_FAILURE;
		}
		for (auto x : folders)
			std::cout << x.second << " \t " << x.first << std::endl;
		exit(EXIT_SUCCESS);
	}

	//Vector of command-line-args
	std::vector<std::string> argsV;
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		getargv(arg, argsV);
	}

	//Check for repeated arguments
	for (size_t i = 0; i < argsV.size(); ++i) {
		for (size_t j = i + 1; j < argsV.size(); ++j) {
			if (argsV[i] == argsV[j]) {
				std::cerr << "\nError: Repeated arguments" << std::endl;
				exit(EXIT_SUCCESS);
			}
		}
	}

	//Switches
	bool s = false, b = false, h = false, z = false, n = false, r = false,
		k = false, help = false, blockSize = false, folder = false, version = false;

	std::vector<std::string> folderName;
	path NamefCheck;
	std::string clusterSize;

	//Find repeated switches and check for unknown switches as well. If found switch set true
	for (size_t i = 0; i < argsV.size(); ++i) {

		//Check for repeated arguments	
		// Similar code used in Garth Santor's class
		std::string str = argsV[i].substr(0, 11);

		if (str == "block-size=") {
			clusterSize = argsV[i].substr(11, argsV[i].length());
			blockSize = true;
		}
		else if (argsV[i] == "help")
			help = true;
		else if (argsV[i] == "version")
			version = true;
		else if (argsV[i] == "s")
			s = true;
		else if (argsV[i] == "b")
			b = true;
		else if (argsV[i] == "h")
			h = true;
		else if (argsV[i] == "z")
			z = true;
		else if (argsV[i] == "k")
			k = true;
		else if (argsV[i] == "r")
			r = true;
		else if (argsV[i] == "n")
			n = true;
		else if (argsV[i].length() > 1) {
			NamefCheck = argsV[i];
			if (!(exists(NamefCheck))) {
				if (!(is_directory(NamefCheck))) {
					std::cerr << "\nError: folder <" << argsV[i] << "> does not exist." << std::endl;
					exit(EXIT_SUCCESS);
				}
			}
			else
				folderName.push_back(NamefCheck.string());
		}
		else {
			std::cout << "Error: unknown switches: <" << argsV[i] << ">";
			return EXIT_FAILURE;
		}
	}

	//Switch validation 
	if (b == true && h == true) {
		std::cout << "Error: cannot use both -b and -h";
		return EXIT_FAILURE;
	}
	if (k == true && blockSize == true) {
		std::cout << "Error: -k and --block-size are incompatible.";
		return EXIT_FAILURE;
	}
	if (z == true && n == true) {
		std::cout << "Error: -n and -z switches are incompatible.";
		return EXIT_FAILURE;
	}

	//Default cluster size
	unsigned int sizeCluster = 4096;
	unsigned int humanRead = 1024;

	//Display only the help listing
	//Printing extracted from Garth Santor's example
	if (help == true) {
		std::cout << "du(c) 2020, Priscilla Marques Peron\n"
			<< "===========================================================\n"
			<< "Version 1.0.0\n\n"
			<< "A disk usage utility inspired by the UNIX du command.\n\n"
			<< "Usage: du [-skhb] [--help] [--version] [--block-size=dddd] [folder]*\n\n"
			"Examples:\n"
			" du\n"
			"  > display the sum of the cluster sizes of each directory\n"
			" starting the cwd\n\n"
			" du folder\n"
			"  > display the sum of the cluster sizes of each directory\n"
			"   starting with 'folder'\n\n"
			" du -h\n"
			"  > display the results in a human readable form\n\n"
			" du -s\n"
			"  > display only the final summary\n\n"
			" du -b\n"
			"  > display in bytes\n\n"
			" du -k\n"
			"  > cluster size is 1024\n\n"
			" du -z\n"
			"  > display the list sorted by size\n\n"
			" du -n\n"
			"  > display the list sorted by name\n\n"
			" du -r\n"
			"  > display the list in reverse order\n\n"
			" du --block-size=dddd\n"
			"  > set the cluster size to the specified integer > 0\n\n"
			" du --help\n"
			"  > displays the help\n\n"
			" du --version \n"
			"  > displays version number in the format d.d.d"
			<< std::endl;;
		exit(EXIT_SUCCESS);
	}
	//Display the version number only in the form d.d.d
	if (version == true) {
		std::cout << myString << std::endl;
		exit(EXIT_SUCCESS);
	}
	//Setting up switches 
	if (k == true)
		blockSizeDefined = 1024;

	if (blockSize == true) {
		blockSizeDefined = stoi(clusterSize, nullptr, 0);
		if (sizeCluster <= 0) {
			std::cerr << "Error: block-size value too small <" << sizeCluster << ">" << std::endl;
			exit(EXIT_SUCCESS);
		}
	}

	//Verify if the user entered a folder to scan or not
	std::string currentFolder = ".";
	std::string scannedFolder;

	if (folderName.empty()) {
		folderName.push_back(currentFolder);
	}

	//Scan the directory
	uintmax_t totalSize = recursive_scan(folderName);

	//Display in bytes, not clusters
	if (b == true) {
		for (size_t i = 0; i < folders.size(); ++i) {
			folders[i].second = folders[i].second * blockSizeDefined;
		}
	}
	double tempVal = 0;
	DiskUsage fileInfo;
	//Make output in human readable format, converting to KiB, MiB, GiB, TB
	if (h == true) {

		for (size_t i = 0; i < folders.size(); ++i) {
			if ((folders[i].second * blockSizeDefined) > pow(humanRead, 4)) {
				tempVal = (folders[i].second * blockSizeDefined) / pow(humanRead, 4);
				fileInfo.character = 'T';
			}
			else if ((folders[i].second * blockSizeDefined) > pow(humanRead, 3)) {
				tempVal = (folders[i].second * blockSizeDefined) / pow(humanRead, 3);
				fileInfo.character = 'G';
			}
			else if ((folders[i].second * blockSizeDefined) > pow(humanRead, 2)) {
				tempVal = (folders[i].second * blockSizeDefined) / pow(humanRead, 2);
				fileInfo.character = 'M';
			}
			else if ((folders[i].second * blockSizeDefined) > pow(humanRead, 1)) {
				tempVal = (folders[i].second * blockSizeDefined) / pow(humanRead, 1);
				fileInfo.character = 'K';
			}
			else {
				tempVal = (double)(folders[i].second * blockSizeDefined) / humanRead;
				fileInfo.character = 'B';
			}
			if (tempVal < 10) {
				if (fmod(tempVal, (int)tempVal) >= 0.01) {
					tempVal = trunc(tempVal) + 0.5;
				}
				else if (fmod(tempVal, (int)tempVal) >= 0.5) {
					tempVal = ceil(tempVal);
				}
			}
			else {
				tempVal = round(tempVal);
			}
			fileInfo.space = tempVal;
			fileInfo.name = folders[i].first;
			vector1.push_back(fileInfo);
		}
	}

	//Display only the root folder, showing the sum of all clusters
	if (s == true) {
		uintmax_t sum = 0;
		std::cout << totalSize << "\t" << scannedFolder << std::endl;
		exit(EXIT_SUCCESS);
	}

	//Sort by cluster or byte size
	if (z == true && h != true) {
		sort(folders.begin(), folders.end(), bySizeP);
	}
	else if (z == true && h == true) {
		sort(vector1.begin(), vector1.end(), bySizeF);
	}

	//Sort by path name
	if (n == true && h != true) {
		sort(folders.begin(), folders.end(), byNameP);
	}
	else if (n == true && h == true) {
		sort(vector1.begin(), vector1.end(), byNameF);
	}

	//Reverse the output order, whether sorted or not
	if (r == true && h != true) {
		reverse(folders.begin(), folders.end());
	}
	else if (r == true && h == true) {
		reverse(vector1.begin(), vector1.end());
	}

	//Print the array after all users requests
	if (h == true) {
		for (auto x : vector1) {

			std::cout << x.space << x.character << " \t " << x.name << std::endl;
		}
	}
	else {
		for (auto x : folders) {
			std::cout << x.second << " \t " << x.first << std::endl;
		}
	}
}
