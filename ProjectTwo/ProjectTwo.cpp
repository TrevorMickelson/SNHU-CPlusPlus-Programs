#include <iostream>
#include <utility>
#include <bits/stdc++.h>
#include <unordered_map>

using namespace std;

// Course object
class Course {
    private:
        string courseNumber;
        string courseTitle;
        vector<string> coursePrerequisites;

    public:
        Course() = default;

        Course(string courseNumber, string courseTitle) {
            this->courseNumber = std::move(courseNumber);
            this->courseTitle = std::move(courseTitle);
        }

        Course(string courseNumber, string courseTitle, vector<string> coursePrerequisites) {
            this->courseNumber = std::move(courseNumber);
            this->courseTitle = std::move(courseTitle);
            this->coursePrerequisites = std::move(coursePrerequisites);
        }

        string getCourseNumber() { return courseNumber; }
        string getCourseTitle() { return courseTitle; }
        vector<string> getCoursePrerequisites() { return coursePrerequisites; }
        bool hasPrerequisite() { return !coursePrerequisites.empty(); }
};

// Global data structures
unordered_map<string, Course> courseMap;
vector<Course> orderedVectorCourses;

// Global functions
void initCoursesFromPath(const string& path);
void loadCourse(string& line,  unordered_map<string, Course>& tempCourses);
void orderVectorCourses();
void printCourses();
void printCourse(const string& courseNumber);

/**
 * The one and only main() method
 */
int main() {
    const string path = "courses.txt";
    cout << "Welcome to the course planner. " << endl;

    int choice = 0;
    string courseNumber;

    while (choice != 4) {
        cout << endl << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  4. Exit" << endl << endl;
        cout << "What would you like to do?";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!courseMap.empty()) {
                    cout << "ERROR: Courses have already been loaded!" << endl;
                    break;
                }

                initCoursesFromPath(path);
                cout << "Courses have been successfully loaded!" << endl;
                break;

            case 2:
                if (courseMap.empty()) {
                    cout << "ERROR: The data structures haven't been loaded yet!" << endl;
                    break;
                }

                printCourses();
                break;

            case 3:
                if (courseMap.empty()) {
                    cout << "ERROR: The data structures haven't been loaded yet!" << endl;
                    break;
                }

                cout << "What course do you want to know about?";
                cin >> courseNumber;
                printCourse(courseNumber);
                break;

            case 4:
                cout << "Thank you for using the course planner!" << endl;
                break;

            default:
                cout << "ERROR: " << choice << " is not a valid option." << endl;
                break;
        }
    }

	return 0;
}

void initCoursesFromPath(const string& path) {
    unordered_map<string, Course> tempCourses;

    string line;
    ifstream file;

    if (file.is_open())
        file.close();

    // Opening file and loading courses accordingly
    file.open(path, ios::out);
    while (getline(file, line)) {
        if (line.empty())
            continue;

        loadCourse(line, tempCourses);
    }
    file.close();

    // Validating if a course can load or not
    // this is based on whether an existing
    // prerequisite is valid
    for (auto& coursePair : tempCourses) {
        Course course = coursePair.second;
        bool canLoadCourse = true;

        if (course.hasPrerequisite()) {
            for (const string& prerequisite : course.getCoursePrerequisites()) {
                if (tempCourses.find(prerequisite) == tempCourses.end()) {
                    canLoadCourse = false;
                    cout << "ERROR: " << prerequisite << " is listed, but is NOT a valid prerequisite!" << endl;
                }
            }
        }

        if (canLoadCourse) {
            courseMap.insert(coursePair);
            orderedVectorCourses.push_back(course);
        }
    }

    // Sorting
    orderVectorCourses();
}

void loadCourse(string& line,  unordered_map<string, Course>& tempCourses) {
    string courseNumber;
    string courseTitle;
    vector<string> coursePrerequisites;

    string token;
    stringstream ss(line);
    int index = 0;

    while (getline(ss, token, ',')) {
        switch (index) {
            case 0:
                courseNumber = token;
                break;

            case 1:
                courseTitle = token;
                break;

            default:
                coursePrerequisites.push_back(token);
                break;
        }
        index++;
    }

    Course course =  *new Course(courseNumber, courseTitle, coursePrerequisites);
    tempCourses.insert({courseNumber, course});
}

/**
 * Because the courses are only ever loaded
 * once, I'm sorting them here using selection
 * sort for faster printing
 */
void orderVectorCourses() {
    for (int i = 0; i < orderedVectorCourses.size(); i++) {
        int smallestIndex = i;

        for (int j = i + 1; j < orderedVectorCourses.size(); j++) {
            Course innerCourse = orderedVectorCourses[j];
            Course smallestIndexCourse = orderedVectorCourses[smallestIndex];

            if (innerCourse.getCourseNumber() < smallestIndexCourse.getCourseNumber())
                smallestIndex = j;
        }

        // Swapping
        Course tempCourse = orderedVectorCourses[i];
        orderedVectorCourses[i] = orderedVectorCourses[smallestIndex];
        orderedVectorCourses[smallestIndex] = tempCourse;
    }
}

void printCourses() {
    cout << "Here is a sample schedule: " << endl << endl;

    for (Course course : orderedVectorCourses) {
        cout << course.getCourseNumber() << ", " << course.getCourseTitle() << endl;
    }
}

void printCourse(const string& courseNumber) {
    auto coursePair = courseMap.find(courseNumber);

    if (coursePair == courseMap.end()) {
        cout << "ERROR: That course id does not exist! Try printing the courses to view available options!" << endl;
        return;
    }

    Course course = coursePair->second;
    cout << course.getCourseNumber() << ", " << course.getCourseTitle() << endl;
    cout << "Prerequisites: ";

    if (!course.hasPrerequisite()) {
        cout << "None" << endl;
        return;
    }

    const vector<string> prerequisites = course.getCoursePrerequisites();

    for (int i = 0; i < prerequisites.size(); i++) {
        const string& prerequisite = prerequisites[i];
        cout << prerequisite;

        if (i == prerequisites.size() - 1) {
            cout << endl;
        } else {
            cout << ", ";
        }
    }
}
