/*
 * ProjectTwo.cpp
 *
 *  Created on: Oct 16, 2026
 *      Author: Mickalia Reid
 *
 *      This code uses the Hash Table Data Structure for the ABCU application
 *      to be used by the Academic Advisors when helping stuents. This code
 *      load the various courses, print a list of the ourses and search
 *      for a requested course.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

// This structure will hold the data for each course.
// Each course has a course number, course name and a list of
// prerequisites.
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;

};

// This function loads the course data from the CSV file and stores the
// courses in a hash table.
bool loadCourses(string fileName, unordered_map<string, Course>& courses) {

	// This opens the file using the filname entered by the user
	ifstream file(fileName);

	// This performs a check to see if the file opened successfully
	if (!file.is_open()) {
		cout << "Error: The file could not be opened. Try again." << endl;
		return false;
	}

	// This creates an empty temporary hash table by using the course
	// number as the key.
	unordered_map<string, Course> newCourses;
	string line;

	// Read each line from the CSV file.
	while (getline(file, line)) {

		// Create a vector to store the values separated by a commas.
		vector<string> tokens;
		string token;
		stringstream ss(line);

		// Separate each line of the CSV file at the commas and store each
		// eparated value in the tokens vector.
		while (getline(ss, token, ',')) {
			tokens.push_back(token);
		}

		// Each line must contain at least a course number and a course name
		if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
			cout << "Error: Invlaid course layout. Try again." << endl;
			return false;
		}

		// Create a new course object
		Course course;

		// Store the first token as the course number
		course.courseNumber = tokens[0];

		// Store the second token as the course Name
		course.courseName = tokens[1];

		// All the remaining tokens are prerequisites
		for (size_t i = 2; i < tokens.size(); i++) {

			// Add each prerequisite to the course.
			if (!tokens[i].empty()) {
				course.prerequisites.push_back(tokens[i]);
			}

		}

		// Check if the course number already exists
		if (newCourses.find(course.courseNumber)!= newCourses.end()) {
			cout << "Error: Duplicate course number." << endl;
			return false;
		}

		// Insert the course into the hash tableand use the course number
		// as the key.
		newCourses[course.courseNumber] = course;


	}

	// Close the file aftr reading the course data
	file.close();

	// Check if any courses were loaded
	if (newCourses.empty()) {
		cout << "Error: The file contains no courses" << endl;
		return false;
	}

	// Validate that each of the prerequisite exists as a course in the
	// hash table.
	for (const auto& entry : newCourses) {

		// Get the course from the hash table
		const Course& course = entry.second;

		// Check each of the prerequisite for the course.
		for (const string& prerequisite : course.prerequisites) {

			// Search for the prerequisite course number.
			if (newCourses.find(prerequisite) == newCourses.end()) {
				cout << "Error: Prerequisite" << prerequisite
						<< " does not exist. Try again." << endl;
				return false;
			}
		}
	}

	// Store the validated courses into the main hash table
	courses = newCourses;

	// Return true because the CSV file had loaded successfully.
	return true;
}

// This function prints all courses in alphanumeric order.
void printCourseList(const unordered_map<string, Course>& courses) {

	// Create a temporary vector to hold the courses.
	vector<Course> sortedCourses;

	// Copy each course from the hash table into the vector.
	for (const auto& entry : courses) {
		sortedCourses.push_back(entry.second);
	}

	// Use selection sort to arrange the courses from the lowest to the
	// highest course number
	for (size_t i=0; i < sortedCourses.size(); i++) {

		// Assume that the current position is the smallest
		size_t minIndex = i;

		// Compare the current course with the remaining courses.
		for (size_t j = i + 1; j < sortedCourses.size(); j++) {

			// Check if another course number is maller
			if (sortedCourses[j].courseNumber < sortedCourses[minIndex].courseNumber) {

				// Save the position of the smaller course
				minIndex = j;
			}
		}

		// Move the smallest course into the correct position.
		swap(sortedCourses[i], sortedCourses[minIndex]);
	}

	// Display the sorted course list
	cout << endl;
	cout << "Here is a sample schedule:" << endl;

	// Print each of the course number and course name
	for (const Course& course: sortedCourses) {
		cout << course.courseNumber << ", " << course.courseName << endl;
	}
}

// This function searches for an individual course and prints it's name and
// prerequisites.
void searchCourse(const unordered_map<string, Course>& courses, string courseNumber) {

	// Search the hash table using the course number.
	auto found = courses.find(courseNumber);

	// Display an error if the course does not exist
	if (found == courses.end()) {
		cout << "The course was not found. Try again" << endl;
		return;
	}

	// Get the course data from the hash table
	const Course& course = found->second;

	// Print the course number and course name.
	cout << endl;
	cout << course.courseNumber << ", " << course.courseName << endl;

	// Check if the course has any prerequisites
	if (course.prerequisites.empty()) {
		cout << "There are no prerequisites." << endl;
	}
	else {
		cout << "Prerequisites:" << endl;

		// Go through each prerequisite for the course
		for (const string& prerequisite : course.prerequisites) {

			// Search for the prerequisite in the hash table.
			auto prereqCourse = courses.find(prerequisite);

			// Print the prerequisite nmber and name.
			if (prereqCourse != courses.end()) {
				cout << prereqCourse->second.courseNumber << ", "
					 << prereqCourse->second.courseName << endl;
			}
		}
	}

}

// This is the main function of the program.
// This allows the acdemic advisor to select from the different menu options.
int main() {

	// Create an empty hash table to store the courses.
	unordered_map<string, Course> courses;

	// keep track of if the data was loaded.
	bool dataLoaded = false;

	// Set the starting menu choice to zero
	string choice = "0";

	// Display a welcome message for the user
	cout << "Welcome to the ABCU course planner." << endl;
	cout << "Please select an option from the menu." << endl;

	// Continue displaying the menu until option 9 is selected by user
	while (choice != "9") {
		cout << endl;
		cout << "1. Load data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl;

		// Ask the user to select an option from the menu
		cout << "What would you like to do?";
		getline(cin, choice);

		// Option 1: Load the Course data file
		if (choice == "1") {
			string fileName;

			// Ask the user to enter the CSV filename
			cout << "Plase enter the file name: ";
			getline(cin, fileName);

			// Call the loadCourses function
			bool loadSuccess = loadCourses(fileName, courses);

			// Check if the file was loaded successfully.
			if (loadSuccess == true) {
				dataLoaded = true;
				cout << "The course data was loaded successfully." << endl;
			}
			else {

				// keep the previously loaded data if a new file fails to load.
				dataLoaded = !courses.empty();
				cout << "The course data could not be loaded." << endl;
			}
		}
		// Option 2: Print all courses in alphanumeric order
		else if (choice == "2") {

			// check if the course file has been loaded.
			if (dataLoaded == false) {
				cout << "Please load a file first." << endl;
			}
			else {
				// call the function to print the sorted list.
				printCourseList(courses);
			}
		}
		// Option 3: Search for a specific course.
		else if (choice == "3") {

			// check if the file has been loaded
			if (dataLoaded == false) {
				cout << "Please load a file first" << endl;
			}
			else {
				string courseNumber;

				// Ask the user for the course number.
				cout << "Please enter the course number: ";
				getline(cin, courseNumber);

				// Search for the requested course
				searchCourse(courses, courseNumber);
			}
		}
		// Option 9: Exit the program
		else if (choice == "9") {
			cout << "Now ending your session. Thank you!" << endl;
		}
		// Display an error for an invalid menu choice
		else {
			cout << "Invalid choice. Please choose from the menu option." << endl;
		}
	}
	// End the program
	return 0;
}


