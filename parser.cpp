#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <stdio.h>

using namespace std;

void computeTimeDifference(struct TIME t1, struct TIME t2, struct TIME *difference);

struct TIME
{
	double seconds;
	int minutes;
	int hours;

	int d;
	int m;
	int y;
};

struct TIME_1
{
	double seconds;
	int minutes;
	int hours;
};

class replicates{ // Class for the replicates on the .json file.

public:

	TIME_1 time_elapsed_rep;

	TIME first_date_rep;
	TIME last_date_rep; 
	string last_status_rep; 

	string walletAddress;
	string resultLink_rep; 
	string chainCallBackData_rep; 
	string chainTaskId_rep; 
	string contributionHash; 
	int credibility;
	int workerWeight;
	string recoverable; 
	string busyComputing; 
};

class date_status{

public:

	TIME first_date_ds;
	TIME last_date_ds;

	string last_status_ds;

	TIME_1 time_elapsed_ds;
};

class parser_obj { // Our general class for the parser.

public:

	string TaskId;
	string chainTaskId_general; 

	vector<replicates> rep_vec;

	int maxExecutionTime;
	int version;
	string tag; 
	string dappType; 
	string dappName; 
	string commandLine; 
	int initializationBlockNumber;
	string currentStatus; 
	int trust;
	string uploadingWorkerWalletAddress;
	string consensus; 

	TIME contributionDeadline;
	TIME revealDeadline;
	TIME finalDeadline; 

	string resultLink_general;
	string chainCallBackData_general;

	date_status my_date_example;
};

struct node_parser
{
	parser_obj data;

	node_parser *next;
};


bool exists(vector<string> example, string str)
{
	for(int i = 0 ; i < example.size() ; i++)
	{
		if(example[i] == str) // exists.
		{
			return true;
		}
	}

	return false;
}


int which_index(vector<string> example, string str)
{
	for(int i = 0 ; i < example.size() ; i++)
	{
		if(example[i] == str) // exists.
		{
			return i;
		}
	}
}

//Multiple parsing functions were created because the characteristics of each line to be parsed can vary.

string return_1(string example) // For parsing strings.
{
	int pos = example.find(": \"");
	string str1 = example.substr(pos + 3);
	string str2 = str1.substr(0, str1.length()-2);
	return str2;
}

string return_1_commandLine(string example) // For parsing strings.
{
	int pos = example.find(": \"");
	string str1 = example.substr(pos + 3);
	string str2 = str1.substr(0, str1.length()-1);
	return str2;
}

int return_2(string example) // For parsing int type variables.
{
	int pos = example.find("\":");
	string str1 = example.substr(pos + 2);
	string str2 = str1.substr(0, str1.length()-1);
	int result = stod(str2);
	return result;
}

string return_2_1(string example) // For parsing strings. Virgülsüz. Used to parse strings like: \"busyComputing\":false" 
{
	int pos = example.find("\":");
	string str1 = example.substr(pos + 2);
	string str2 = str1.substr(0, str1.length());
	return str2;
}

string return_2_2(string example) // For parsing strings. Used to parse strings like: \"recoverable\":false,". Similar to return_2. However, a string is returned.
{
	int pos = example.find("\":");
	string str1 = example.substr(pos + 2);
	string str2 = str1.substr(0, str1.length()-1);
	return str2;
}

TIME return_3(string example) // For parsing dates.
{
	TIME result;

	int pos = example.find(": \"");
	string str1 = example.substr(pos + 3);
	string str2 = str1.substr(0, str1.length()-2);

	result.y = stod(str2.substr(0,4));
	result.m = stod(str2.substr(5,2));
	result.d = stod(str2.substr(8,2));

	result.hours = stod(str2.substr(11,2));
	result.minutes = stod(str2.substr(14,2));
	result.seconds = stod(str2.substr(17,6));

	return result;
}

const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // To store number of days in all months from January to December. 

int countLeapYears(TIME d) // Counts number of leap years before the given date.
{ 
	int years = d.y; 

	if (d.m <= 2) // Check if the current year needs to be considered for the count of leap years or not 
		years--; 

	return years / 4 - years / 100 + years / 400; // An year is a leap year if it is a multiple of 4, multiple of 400 and not a multiple of 100.
} 

// This function returns number of days between two given dates.
int getDifference(TIME dt1, TIME dt2) 
{ 
	// COUNT TOTAL NUMBER OF DAYS BEFORE FIRST DATE 'dt1' 

	long int n1 = dt1.y*365 + dt1.d;  // Initialize count using years and day. 

	// Add days for months in given date 
	for (int i=0; i<dt1.m - 1; i++) 
	{
		n1 += monthDays[i]; 
	}

	n1 += countLeapYears(dt1); // Since every leap year is of 366 days, add a day for every leap year.

	// SIMILARLY, COUNT TOTAL NUMBER OF DAYS BEFORE 'dt2' 

	long int n2 = dt2.y*365 + dt2.d; 
	for (int i=0; i<dt2.m - 1; i++) 
		n2 += monthDays[i]; 
	n2 += countLeapYears(dt2); 

	return (n2 - n1); // Return difference between two counts 
} 

void computeTimeDifference_1(struct TIME t1, struct TIME t2, struct TIME_1 *difference)
{
	if(t2.seconds > t1.seconds)
	{
		--t1.minutes;
		t1.seconds += 60;
	}
	difference->seconds = t1.seconds - t2.seconds;
	if(t2.minutes > t1.minutes)
	{
		--t1.hours;
		t1.minutes += 60;
	}
	difference->minutes = t1.minutes-t2.minutes;
	difference->hours = t1.hours-t2.hours;
}

void computeTimeDifference(struct TIME t1, struct TIME t2, struct TIME_1 *difference) // Computing ((beginning time of t1) - t2) and putting the result on l2.
{ // t1: bigger date, t2: smaller date

	if(getDifference(t2, t1) == 0) // If the dates have the same year, month and day.
	{
		if(t2.seconds > t1.seconds)
		{
			--t1.minutes;
			t1.seconds += 60;
		}
		difference->seconds = t1.seconds - t2.seconds;
		if(t2.minutes > t1.minutes)
		{
			--t1.hours;
			t1.minutes += 60;
		}
		difference->minutes = t1.minutes-t2.minutes;
		difference->hours = t1.hours-t2.hours;
	}

	else if(getDifference(t2, t1) == 1) // If t1.day = t2.day + 1. Then, ((t1's hour, min, sec) - beginning time of t1) + ((beginning time of t1) - t2) is calculated.
	{
		TIME lz; // Beginning of t2.
		lz.hours = 24;
		lz.minutes = 0;
		lz.seconds = 0;

		TIME l1; // Copying t1 by value.

		l1.hours = t1.hours;
		l1.minutes = t1.minutes;
		l1.seconds = t1.seconds;

		TIME_1 l2;

		computeTimeDifference_1(lz, t2, &l2); // Computing ((beginning time of t1) - t2) and putting the result on l2.

		// Doing the addition operation between two amounts to find the net distance between two dates (while also minding the leap figures when adding minutes and seconds).

		double s1 = l1.seconds + l2.seconds;
		if(s1 > 60)
		{
			int div1 = s1 / 60;
			int remainder1 = s1 - (div1 * 60);
			s1 = remainder1;

			l1.minutes = div1 + l1.minutes;
		}

		int s2 = l1.minutes + l2.minutes;
		if(s2 > 60)
		{
			int div2 = s2 / 60;
			int remainder2 = s2 - (div2 * 60);

			s2 = remainder2;

			l1.hours = div2 = l1.hours;
		}

		int s3 = l1.hours + l2. hours;

		TIME l3;

		// Putting the fresh new results to l3.
		l3.seconds = s1;
		l3.minutes = s2;
		l3.hours = s3;

		if(l3.hours == 23 && l3.minutes == 59 && l3.seconds == 60)
		{
			l3.hours = 24;
			l3.minutes = 0;
			l3.seconds = 0;
		}
	}
	else
	{
	}
}

int main()
{
	parser_obj my_obj; // Our parser object.

	vector<parser_obj> my_parser_vec_1;

	replicates my_rep; // Our replicates object.
	date_status my_date_stat; // Our date status object.

	ifstream reader_1;

	vector<string> file_name_vec;

	// Name of the file to be parsed.

	reader_1.open("0_tasks.txt");

	string sub_line;
	while(getline(reader_1,sub_line))
	{
		file_name_vec.push_back(sub_line);
	}
	// opening the file.
	reader_1.close();

	vector<string> all_lines; // Vector keeping all the lines read from the stream. This is especially useful when we want to keep track of the previous lines to make sure that we are parsing the correct line at the correct time.

	TIME_1 elapser_rep;
	TIME_1 elapser_ds;

	/*
	while(!reader_1) // This loop works when the entered file name is not found. Program is terminated.
	{
	return 0;
	}
	*/

	ofstream writer_1;
	writer_1.open("output_code.arff"); // Outputting the result to Weka format, which is .arff

	writer_1 << "@relation " << endl; // For weka format.
	writer_1 << "" << endl; // For weka format.


	vector<string> consensus_vec;

	vector<string> walletAddress_vec;

	vector<string> all_status_vec;


	int count = 0;

	while(count < 2063) // 2063.
	{
		string file_name = to_string(count) + "_" + file_name_vec[count] + ".json";
		reader_1.open(file_name);
		string line;

		while(getline(reader_1,line))
		{
			all_lines.push_back(line); // Pushing the line to the vector.

			istringstream stream_1(line);

			if( (line.find("date") != string::npos) && (all_lines[all_lines.size()-2].find("{") != string::npos) && (all_lines[all_lines.size()-3].find("dateStatusList") != string::npos) )
			{
				my_obj.my_date_example.first_date_ds = return_3(line);

				if(all_lines[all_lines.size()-4].find("null") == string::npos){
					my_obj.finalDeadline = return_3(all_lines[all_lines.size()-4]);
				}

				if(all_lines[all_lines.size()-5].find("null") == string::npos){
					my_obj.currentStatus = return_1(all_lines[all_lines.size()-5]);
				}
				else{
					my_obj.currentStatus = "null";
				}

				if(all_lines[all_lines.size()-6].find("null") == string::npos){
					my_obj.revealDeadline = return_3(all_lines[all_lines.size()-6]);
				}

				my_obj.trust = return_2(all_lines[all_lines.size()-7]);
				my_obj.chainTaskId_general = return_1(all_lines[all_lines.size()-8]);
			}

			else if( ( line.find("date\":") != string::npos) && (all_lines[all_lines.size()-3].find("statusChangeList") != string::npos) && (all_lines[all_lines.size()-2].find("{") != string::npos) ) // first date (in replica section)
			{
				my_rep.first_date_rep = return_3(line);
				my_rep.chainTaskId_rep = return_1(all_lines[all_lines.size()-5]); // chainTaskId (in replica section)

				if(all_lines[all_lines.size()-4].find("null") == string::npos){
					my_rep.resultLink_rep = return_1(all_lines[all_lines.size()-4]); // resultLink (in replica section). it was return2_2
				}
				else{
					my_rep.resultLink_rep = "null";
				}
			}

			else if(( line.find("busyComputing\":") != string::npos)  &&  (all_lines[all_lines.size()-2].find("chainCallbackData") != string::npos) && (all_lines[all_lines.size()-3].find("walletAddress") != string::npos) && (all_lines[all_lines.size()-4].find("contributionHash") != string::npos) )
			{
				my_rep.busyComputing = return_2_1(line); // busyComputing (in replica section)

				if(all_lines[all_lines.size()-2].find("null") == string::npos){
					my_rep.chainCallBackData_rep = return_1(all_lines[all_lines.size()-2]); // chainCallBackData (in replica section). it was return 2_2
				}
				else
				{
					my_rep.chainCallBackData_rep = "null";
				}

				my_rep.walletAddress = return_1(all_lines[all_lines.size()-3]); // walletAddress (in replica section)

				if( exists(walletAddress_vec, my_rep.walletAddress) == false )
				{
					walletAddress_vec.push_back(my_rep.walletAddress); // pushing all wallet addresses. We will use it when pressing results.
				}

				my_rep.contributionHash = return_1(all_lines[all_lines.size()-4]); // contributionHash (in replica section)
				my_rep.credibility = return_2(all_lines[all_lines.size()-5]); // credibility (in replica section)
				my_rep.recoverable = return_2_2(all_lines[all_lines.size()-6]); // recoverable (in replica section)
				my_rep.workerWeight = return_2(all_lines[all_lines.size()-7]); // workerWeight (in replica section)

				my_rep.last_status_rep = return_1_commandLine(all_lines[all_lines.size()-10]); // last status (in replica section)

				if(exists(all_status_vec,my_rep.last_status_rep) == false)
				{
					all_status_vec.push_back(my_rep.last_status_rep);
				}

				if( all_lines[all_lines.size()-12].find("txHash") != string::npos)
				{
					my_rep.last_date_rep = return_3(all_lines[all_lines.size()-16]); // last date (in replica section)
				}
				else
				{
					my_rep.last_date_rep = return_3(all_lines[all_lines.size()-12]); // last date (in replica section)
				}
				// ------
				computeTimeDifference(my_rep.last_date_rep, my_rep.first_date_rep, &elapser_rep);
				my_rep.time_elapsed_rep = elapser_rep; // Elapsed time for the replica.
				my_obj.rep_vec.push_back(my_rep); // Pushing the replica to the replica vector of the parser object.
			}

			else if( (line.find("initializationBlockNumber") != string::npos) && ( all_lines[all_lines.size()-2].find("maxExecutionTime") != string::npos ) && ( all_lines[all_lines.size()-3].find("]") != string::npos ) &&  (all_lines[all_lines.size()-4].find("}") != string::npos) ) 
			{
				my_obj.initializationBlockNumber = return_2(line);
				my_obj.maxExecutionTime = return_2(all_lines[all_lines.size()-2]);
				my_obj.my_date_example.last_status_ds = return_1_commandLine(all_lines[all_lines.size()-5]);  // Last status at date status.
				my_obj.my_date_example.last_date_ds = return_3(all_lines[all_lines.size()-6]); // Last date at date status.
				computeTimeDifference(my_obj.my_date_example.last_date_ds, my_obj.my_date_example.first_date_ds, &elapser_ds);
				my_obj.my_date_example.time_elapsed_ds = elapser_ds; // Elapsed time for the date status.
			}

			else if( (line.find("commandLine") != string::npos) && ( all_lines[all_lines.size()-2].find("uploadingWorkerWalletAddress") != string::npos ) && ( all_lines[all_lines.size()-3].find("tag") != string::npos ) &&  (all_lines[all_lines.size()-4].find("id") != string::npos) )
			{
				if(line.find("null") == string::npos){
					my_obj.commandLine = return_1_commandLine(line);
				}
				else{
					my_obj.commandLine = "null";
				}

				if( all_lines[all_lines.size()-2].find("null") == string::npos){
					my_obj.uploadingWorkerWalletAddress = return_1( all_lines[all_lines.size()-2]);
				}
				else{
					my_obj.uploadingWorkerWalletAddress = "null";
				}

				if(all_lines[all_lines.size()-3].find("null") == string::npos){
					my_obj.tag = return_1(all_lines[all_lines.size()-3]);
				}
				else{
					my_obj.tag = "null";
				}

				my_obj.TaskId = return_1(all_lines[all_lines.size()-4]);

				if(all_lines[all_lines.size()-5].find("null") == string::npos){
					my_obj.dappName = return_1(all_lines[all_lines.size()-5]);
				}
				else{
					my_obj.dappName = "null";
				}

				if(all_lines[all_lines.size()-6].find("null") == string::npos){
					my_obj.dappType = return_1(all_lines[all_lines.size()-6]);
				}
				else{
					my_obj.dappType = "null";
				}

				if(all_lines[all_lines.size()-7].find("null") == string::npos){
					my_obj.resultLink_general = return_1(all_lines[all_lines.size()-7]);  // resultLink property of the parser object.
				}
				else{
					my_obj.resultLink_general = "null";
				}

				if(all_lines[all_lines.size()-8].find("null") == string::npos) {
					my_obj.consensus = return_1(all_lines[all_lines.size()-8]);
					consensus_vec.push_back(my_obj.consensus);
				}
				else{
					my_obj.consensus = "null";
					consensus_vec.push_back(my_obj.consensus);
				}

				if(all_lines[all_lines.size()-9].find("null") == string::npos){
					my_obj.chainCallBackData_general = return_1(all_lines[all_lines.size()-9]); // chainCallBackData property of the parser object.
				}
				else{
					my_obj.chainCallBackData_general = "null";
				}

				if(all_lines[all_lines.size()-10].find("null") == string::npos){
					my_obj.contributionDeadline = return_3(all_lines[all_lines.size()-10]);
				}

				my_obj.version = return_2(all_lines[all_lines.size()-11]);
			}
		}

		all_lines.clear();

		my_parser_vec_1.push_back(my_obj);

		my_obj.rep_vec.clear();

		cout << count << " is finished." << endl;

		count++;

		reader_1.close();
	}

	file_name_vec.clear();

	vector<string> status_vec; // Keeps the last status of each replica from the specified task.
	vector<string> contribution_hash_vec; //Keeps the contribution hashes of each replica from the specified task.
	vector<string> small_wallet; // Keeps the wallet addresses of each replica from the specified task.

	int index_1;


	for(int t = 0; t < walletAddress_vec.size(); t++) // for weka format.
	{
		writer_1 << "@attribute " << "worker" << t << " {0,1,2,3}" << endl;
	}

	writer_1 << "@attribute failed {Y,N}" << endl; // for weka format.
	writer_1 << "" << endl; // for weka format.
	writer_1 << "@data" << endl; // for weka format.

	for(int i = 0; i < my_parser_vec_1.size() ; i++)
	{
		for(int k = 0; k < my_parser_vec_1[i].rep_vec.size() ; k++)
		{
			small_wallet.push_back(my_parser_vec_1[i].rep_vec[k].walletAddress);
			status_vec.push_back(my_parser_vec_1[i].rep_vec[k].last_status_rep);
			contribution_hash_vec.push_back(my_parser_vec_1[i].rep_vec[k].contributionHash);
		}

		// comparison and printing to the text begins.

		for(int j = 0; j < walletAddress_vec.size(); j++)
		{
			if(exists(small_wallet,walletAddress_vec[j]) ) // It is in the group. It will be either (0), (1) or (2).
			{

				index_1 = which_index(small_wallet,walletAddress_vec[j]);


				if(contribution_hash_vec[index_1] == consensus_vec[i])
				{
					writer_1 << "0,"; // Same result with the consensus --> press (0)
				}

				else if(contribution_hash_vec[index_1] != consensus_vec[i])
				{
					if(contribution_hash_vec[index_1] == "" && status_vec[index_1] != "COMPLETED" && status_vec[index_1] != "RESULT_UPLOADED")
						writer_1 << "2,"; // Different result with the consensus --> press (1)

					else
						writer_1 << "1,";
				}
			}

			else
			{
				writer_1 << "3,"; // did not join --> press (-1)
			}
		}

		if(my_parser_vec_1[i].currentStatus == "FAILED")
		{
			writer_1 << "Y" << endl;

		}
		else
		{
			writer_1 << "N" <<endl;
		}


		//  comparison and printing to the text ends.

		small_wallet.clear();
		status_vec.clear();
		contribution_hash_vec.clear();

		cout << "Task: " << i << " is written." << endl;
	}

	reader_1.close();
	writer_1.close();

	return 0;
}
