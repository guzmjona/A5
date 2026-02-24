/*
Lauren Gliane
do we need IDS?
Jonathan Guzman
*/
/* This is a skeleton code for two-pass multi-way sorting. You can make modifications as long as you meet 
   all question requirements. You are also free to change the return type and arguments as needed. */

#include <bits/stdc++.h>
#include "record_class4.h"

using namespace std;

#define buffer_size 250 //defines how many pages are available in the Main Memory 


Records buffers[buffer_size]; 
vector<string> temp_files;

/***TODO: You may need to modify the return type and arguments of the following functions based on your implementation.***/


//Function for PASS 1
// TODO: Complete the following function to sort the buffers and store the sorted records into a temporary file (Runs).
void Sort_Buffer(fstream& main_info_file){
    int run_num = 0;
    while(!main_info_file.eof()){
        //read info to records
        int count = 0;
        //for every slot in the buffer
        for(int i = 0; i < buffer_size; i++){
            //grab info
            buffers[i] = Grab_Emp_Record(main_info_file);
            //if there's no info then break out and move on
            cout << i << ", ";
            if(buffers[i].no_values == -1){
                break;
            }
            count++;
        }

        cout << buffers[0].emp_record.name << endl;
        cout << buffers[1].emp_record.name << endl;
        cout << buffers[2].emp_record.name << endl;

        if(count == 0){
            //DEBUGGIN
            cout << "WE DID NOT READ ANYTHING WHY NOTT WTFFFFFFF " << endl;
            break;
        }

        //sort based in id - id_1 < id_2
        for(int i = 0; i < count - 1; i++){
            for(int j = 0; j < count - i - 1; j++){
                if(buffers[j].emp_record.id > buffers[j + 1].emp_record.id){
                    Records temp = buffers[j];
                    buffers[j] = buffers[j + 1];
                    buffers[j + 1] = temp;
                }
            }
        }
        cout << "Record have been sorted for the run "<< count <<endl;
        cout << "Record have been sorted for the run "<< count <<endl;
        cout << "Record have been sorted for the run "<< count <<endl;
        cout << "Record have been sorted for the run "<< count <<endl;

        //once sorted write into a temporary file
        string filename = "run_" + to_string(run_num) + ".txt";
        temp_files.push_back(filename);
        fstream outFile(filename, ios::out);
        for(int i = 0; i < count; i++){
            outFile << buffers[i].emp_record.id << ","
                   << buffers[i].emp_record.name << ","
                   << buffers[i].emp_record.bio << ","
                   << buffers[i].emp_record.manager_id << endl;
        }
        outFile.close();
        run_num++;
        cout << "Record have been writter for the run "<< run_num <<endl;
        cout << "Record have been writter for the run "<< run_num <<endl;
        cout << "Record have been writter for the run "<< run_num <<endl;
        cout << "Record have been writter for the run "<< run_num <<endl;
    }
    //int mom = 0;
    //cin >> mom;
    return;
}

// TODO: Complete the following function to store the sorted results from PASS 2 into EmpSorted.csv.
void PrintSorted(fstream &sourted_out_file, Records& one_record){

    //write to sorted out 
    sourted_out_file << one_record.emp_record.id << ","
            << one_record.emp_record.name << ","
            << one_record.emp_record.bio << ","
            << one_record.emp_record.manager_id << endl;

    return;
}

//Function for PASS 2
// TODO: Complete the following function to merge the sorted temporary files ('runs') and store the final result in EmpSorted.csv using PrintSorted().
void Merge_Runs(fstream &sourted_out_file){
    int num_runs = temp_files.size();
    //array to pointes that points to the files
    fstream* temp_files_fstream = new fstream[num_runs];
    Records* current_records = new Records[num_runs];
    bool* has_data = new bool[num_runs];
    

    //open all the temp files and read the first record aka the one with the lowest id
    for(int i = 0; i < num_runs; i++){
        temp_files_fstream[i].open(temp_files[i], ios::in);
        string line;
        if (getline(temp_files_fstream[i], line)) {
            stringstream ss(line);
            string word;
            
            getline(ss, word, ',');
            current_records[i].emp_record.id = stoi(word);
            getline(ss, word, ',');
            current_records[i].emp_record.name = word;
            getline(ss, word, ',');
            current_records[i].emp_record.bio = word;
            getline(ss, word, ',');
            current_records[i].emp_record.manager_id = stoi(word);
            
            has_data[i] = true;
        } else {
            has_data[i] = false;
        }
    }

    //get smallest id from temp files and output it
    while (true) {
        //find smallest record
        int smallest_index = -1;
        int smallest_id = INT_MAX;
        for (int i = 0; i < num_runs; i++) {
            if (has_data[i] && current_records[i].emp_record.id < smallest_id) {
                smallest_id = current_records[i].emp_record.id;
                smallest_index = i;
            }
        }
        
        if(smallest_index == -1) break; //no more records
        
        //write out smallest record
        PrintSorted(sourted_out_file, current_records[smallest_index]);
        
        //read next record from that temp file
        string line;
        if (getline(temp_files_fstream[smallest_index], line)) {
            stringstream ss(line);
            string word;
            
            getline(ss, word, ',');
            current_records[smallest_index].emp_record.id = stoi(word);
            getline(ss, word, ',');
            current_records[smallest_index].emp_record.name = word;
            getline(ss, word, ',');
            current_records[smallest_index].emp_record.bio = word;
            getline(ss, word, ',');
            current_records[smallest_index].emp_record.manager_id = stoi(word);
        } else {
            has_data[smallest_index] = false;
        }
    }
    
    // Close all files
    for (int i = 0; i < num_runs; i++) {
        temp_files_fstream[i].close();
    }
    sourted_out_file.close();
    
    // Clean up dynamic arrays
    delete[] temp_files_fstream;
    delete[] current_records;
    delete[] has_data;

    return;
}



int main() {

    fstream empin;     //Open file streams to read and write the Employee_p1.csv

    empin.open("Employee_p1.csv", ios::in);  //Opening out the Employee_p1.csv that we want to sort

   
    fstream SortOut; //Open file streams to read and write the EmpSorted.csv
    SortOut.open("EmpSorted.csv", ios::out | ios::app);  //Creating the EmpSorted.csv file where we will store our sorted results


    //TO DO: PASS 1, Create sorted runs for Employee_p1.csv using Sort_Buffer()
    Sort_Buffer(empin);
    empin.close();
    cout << "Sorting has been done" << endl;
    cout << "Sorting has been done" << endl;
    cout << "Sorting has been done" << endl;
    cout << "Sorting has been done" << endl;
    
    //TO DO: PASS 2, Use Merge_Runs() to sort the runs and generate EmpSorted.csv
    Merge_Runs(SortOut);
    cout << "mergin has been done" << endl;
    cout << "mergin has been done" << endl;
    cout << "mergin has been done" << endl;
    cout << "mergin has been done" << endl;
    //PrintSorted(&SortOut);

    //Please delete the temporary files (runs) after you've sorted the Employee_p1.csv
    for (const string& filename : temp_files) {
        remove(filename.c_str());
    }

    cout << "checking to make sure it works good" << endl;
    //open sorted file
    empin.open("EmpSorted.csv", ios::in); 

    //parse the first line and only read the id
    string line;
    vector<int> my_ids;
    getline(empin, line);
    stringstream ss(line);
    string word;  
    getline(ss, word, ',');
    my_ids.push_back(stoi(word));

    //read the rest of the sorted file and check that the previos is less than the next one
    int counter = 0;
    while(getline(empin, line)){
        stringstream ss(line);
        string word;  
        getline(ss, word, ',');
        my_ids.push_back(stoi(word));
        counter++;
        if(my_ids[counter-1] > my_ids[counter]){
            cout << "we found 2 ids in the wrong place" << endl;
            cout << "row: " << counter-1 << " and " << counter << "are swapped" << endl;
            break;
        }
    }

    return 0;
}
