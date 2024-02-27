# Attendance_Monitoring_Using_Biometry_Absent_SMS_Notification_and_Data_Logging_on_Google_Sheets

The attendance is taken in every organization. Traditional approach for attendance is, professor calls student name & record attendance. For each lecture this is wastage of time. To avoid these losses, 
we are about to use automatic process which is based on Biometry. In this project approach, we are using Biometry system. In the first phase, the fingerprint is fetched and pre-processing is done and the 
fingerprint module waits for the stop signal sent by the NodeMCU. In the second phase, the data is sent to Wi-Fi module and it uploads it to a google spreadsheet. The attendance of students is marked as
either ‘1’ or ‘0’ i.e. either present (1) or absent (0). It is designed to be reliable and low power. It is compact in size and it is designed to replace traditional approach for attendance marking.

Key Features:
        1. Biometry: The system consists of a fingerprint scanner which helps in quick collection of biometric data which is unique to each user. 
        2. Internet connectivity: It consists of a Wi-Fi module through which internet connectivity is established easily.   
        3. Absent notification: The system has a GSM MODEM with which an SMS can be sent to parents in case of absence of students. 
        4. User interface: The system has display, buzzer and LED indicators which form a simple yet effective user interface.         
        5. Cloud database: The system uploads all the data to a cloud-based application hence the accessibility of data is made easy.
    
Benefits of Using Attendance Management System:
        1. No Fuss in Workflow Management. 
        2. Paperless Work Environment. 
        3. Real-Time Tracking. 
        4. Reliable Data. 
        5.Enhancing Productivity. 
        6. Maintain Company Culture. 
        7. Personalized and Flexible. 
        8. Cost-Effective.
 How to make this Project:-
           Schematics:
           ![image](https://github.com/Vignesh830/Attendance_Monitoring_Using_Biometry_Absent_SMS_Notification_and_Data_Logging_on_Google_Sheets/assets/159744719/5ad952ca-3613-440b-9aca-216c5542c8e0)
           Block Diagram:
           ![image](https://github.com/Vignesh830/Attendance_Monitoring_Using_Biometry_Absent_SMS_Notification_and_Data_Logging_on_Google_Sheets/assets/159744719/65a56ca3-7f31-4d11-b238-208e769244fd)
 Working Principle:
            he proposed system of attendance monitoring works in real time with no manual supervision. The basic concepts used are sensor data collection, comparison and verification; user interface, decision making, wireless communication and alerting. The system acts as one common unit that coordinates between students, school/college staff, management and parents. The students can log in their attendance just by placing the finger on the fingerprint sensor while entering the class. This eliminates the manual process of attendance collection thus saving a good amount of time. The attendance data is directly uploaded to a web portal school/college immediately with the help of Wi-Fi. This prevents loss of data due to human errors and also helps in maintaining the records safely. With the help of collected data, analytics can also be produced which helps in behavioural study of the students. The system also consists of a feature which sends an SMS to the parents in case of absence of the students. The system waits for a pre-set amount of time after which the particular student will be marked absent and an immediate SMS will be sent to the parents informing the same. This helps the parents to monitor their children in a better way. It also ensures the safety of the students.

Program Flow for Node-MCU:
        1. Power on 
        2. Load timetable 
        3. Connect to Wi-Fi and internet 
        4. Get date, day and updated time from NTP server 
        5. Compare present time with today’s timetable 
        6. If it matches, a message is sent to Arduino Nano to take up attendance with the respective period number 
        7. Then, it keeps checking the time to see if it has crossed the maximum allowable late entry 
        8. Once the minutes are over, it sends a message to Arduino Nano to stop taking attendance and to submit the latest attendance to it 
        9. It receives the attendance data of the latest period and uploads the same to Google Sheets 
        10. Goes back to the loop to check the time for the next period
Program Flow for Arduino Nano:
        1. Power on 
        2. Update the display, LEDs and buzzer 
        3. Check continuously if there is a message from the NODE MCU to take attendance 
        4. Once message received, enable fingerprint scanner, update display with the period number and check for scanned data 
        5. If scanned, give feedback whether the scan was successful or not to the user via display, LEDs and buzzer 
        6. Store the user’s attendance data in a temporary file 
        7. Check if there is message from NODE MCU asking to submit attendance 
        8. If yes, disable fingerprint scanner, update the display and send the temporary file containing the attendance of the period to NODE MCU 
        9. If no, continue taking attendance

        
        
 

        
