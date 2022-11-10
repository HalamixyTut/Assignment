import java.sql.*;
import oracle.jdbc.*;
import java.math.*;
import java.io.*;
import java.awt.*;
import oracle.jdbc.pool.OracleDataSource;
import java.util.Scanner;
import java.util.stream.Stream;
import java.util.regex.Pattern;
import java.util.stream.Stream;

import javax.print.event.PrintJobListener;

public class project2 {
    public static OracleDataSource ds;
    public static Connection conn;
    public static Statement stmt;

    public static void main(String args[]) throws SQLException {
    	//connect to oracle database
        ds = new oracle.jdbc.pool.OracleDataSource();
        ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
        conn = ds.getConnection("yli500", "Lyq19960425");
        stmt = conn.createStatement();
        String number1 = "";
        //enter number to use function
        while (!number1.equals("0")) {
            showtable();
            Scanner scanner = new Scanner(System.in);
            number1 = scanner.nextLine();
            //the number should between 0 - 13
            if(!number1.equals("1") && !number1.equals("2") && !number1.equals("3") && !number1.equals("4")
            && !number1.equals("5") && !number1.equals("6") && !number1.equals("7") && !number1.equals("8")  
            && !number1.equals("9") && !number1.equals("10") && !number1.equals("11") && !number1.equals("12")
            && !number1.equals("13") && !number1.equals("0")){
                System.out.println("please enter correct number");
                continue;
            }
            checknumber(number1);
        }
        conn.close();
    }
    //interface table
    public static void showtable() {
        System.out.println(" ------------------------------------------------- ");
        System.out.println("|                    welcome                       |");
        System.out.println("|	     Student Registration System           |");
        System.out.println("|	     Enter 1 for show student              |");
        System.out.println("|	     Enter 2 for show courses              |");
        System.out.println("|	     Enter 3 for show prerequisites        |");
        System.out.println("|	     Enter 4 for show course credit        |");
        System.out.println("|	     Enter 5 for show classes              |");
        System.out.println("|	     Enter 6 for show score grade          |");
        System.out.println("|	     Enter 7 for show enrollments          |");
        System.out.println("|	     Enter 8 for show logs                 |");
        System.out.println("|	     Enter 9 for show student in class     |");
        System.out.println("|	     Enter 10 for show prerequisite courses|");
        System.out.println("|	     Enter 11 for enroll student into class|");
        System.out.println("|	     Enter 12 for drop student from class  |");
        System.out.println("|	     Enter 13 for delete student           |");
        System.out.println("|	     Enter 0 for exit                      |");
        System.out.println(" ------------------------------------------------- ");
        System.out.println("enter the number:");
    }

    public static void checknumber(String number) throws SQLException {
    	//show student table
        if (number.equals("1")) {
            showstudent();
        }
        //show courses table
        if (number.equals("2")) {
            showcourses();
        }
        //show prerequisites table
        if (number.equals("3")) {
             showprerequisites();
        }
        //show course credit table
        if (number.equals("4")) {
            showcoursecredit();
        }
        //show classes table
        if (number.equals("5")) {
            showclasses();
        }
        //show score grade table
        if (number.equals("6")) {
            showscoregrade();
        }
        //show enrollments table
        if (number.equals("7")) {
            showgenrollments();
        }
        //show logs table
        if (number.equals("8")) {
            showlogs();
        }
        //show student by classid
        if (number.equals("9")) {
            showstudentbyclassid();
        }
        //show prerequisites by classid
        if (number.equals("10")) {
            showprerequisries();
        }
        //enroll students
        if (number.equals("11")) {
            enrollstudent();
        }
        //drop students
        if (number.equals("12")) {
            dropstudent();
        }
        //delete students from students table
        if (number.equals("13")) {
            deletestudent();
        }
    }
    
    public static void showstudent() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
            //use project.show_students function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_students; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
	
	    System.out.println("B#         first_name  last_name st_level gpa   email            birthdate");
	    //get all data in student table
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" + rs.getString(3) + "\t"
                        + rs.getString(4) + "\t" + rs.getString(5) + "\t"
                        + rs.getString(6) + "\t" + rs.getString(7) + "\t");
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showcourses() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
	    //use project.show_courses function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_courses; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
	    System.out.println("dept_code     course#   title");
	    //show database
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" + rs.getString(3));
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showprerequisites() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
            //use project.show_prerequisites function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_prerequisites; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
	    System.out.println("dept_code      course#          pre_dept_code   pre_course#");
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" + rs.getString(3)
                        + "\t" + rs.getString(4));
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showcoursecredit() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
	    //use project.show_course_credit function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_course_credit; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);
	    System.out.println("course#         credit ");
            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);

            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2));
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showclasses() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
	    //use project.show_classes function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_classes; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);
	    System.out.println("classid      dept_code course# sect#    year   semester limit  class_size  room");
            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);

            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2)+ "\t" +rs.getString(3)
                        + "\t" + rs.getString(4) + "\t" + rs.getString(5)
                        + "\t" + rs.getString(6) + "\t" + rs.getString(7) 
                        + "\t" + rs.getString(8) + "\t" + rs.getString(9)
                        );
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showscoregrade() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
	    //use project.show_score_grade function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_score_grade; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
	    System.out.println("score       lgrade");
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2));
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showgenrollments() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
	    //use project.show_g_enrollments function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_g_enrollments; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
 	    System.out.println("B#           classid  score");
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" +
                        rs.getString(3));
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static void showlogs() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();
	    //use project.show_logs function in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_logs; end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
	    System.out.println("log#    user         time                table_name     operation  B#    classid");
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" +rs.getString(3)
                        + "\t" +rs.getString(4) + "\t" +rs.getString(5)
                        + "\t" +rs.getString(6)
                        );
            }
            cs.close();
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }

    public static void showstudentbyclassid() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();

            stmt.executeUpdate("begin dbms_output.enable(); end;");
	    //use project.showStudentsByClassid() in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.showStudentsByClassid(?); end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            BufferedReader readKeyBoard;
            String classid;
            //enter classid
            readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("Please enter ClassID: ");
            classid = readKeyBoard.readLine();
            cs.setString(2, classid);

            cs.execute();
            ResultSet rs = (ResultSet) cs.getObject(1);
	    System.out.println("B#        first_name  last_name");
            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" + rs.getString(3));
            }

            try (CallableStatement call = conn.prepareCall(
                    "declare "
                            + "  num integer := 1000;"
                            + "begin "
                            + "  dbms_output.get_lines(?, num);"
                            + "end;")) {
                call.registerOutParameter(1, Types.ARRAY,
                        "DBMSOUTPUT_LINESARRAY");
                call.execute();

                Array array = null;
                try {
                    array = call.getArray(1);
                    if (((Object[]) array.getArray()).length != 0) {
                        Stream.of(((Object[]) array.getArray())[0]).forEach(System.out::println);
                    }
                } finally {
                    if (array != null)
                        array.free();
                }
                call.close();
                cs.close();
            }
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
    
    public static boolean isNumber(String str) {
	Pattern pattern = Pattern.compile("[0-9]*");
	return pattern.matcher(str).matches();
    }    

    public static void showprerequisries() throws SQLException {
        try {
            //OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            //ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            //Connection conn = ds.getConnection("yli500", "Lyq19960425");

            //Statement stmt = conn.createStatement ();
            //use project.show_prerequisite() in plsql
            CallableStatement cs = conn.prepareCall("begin ? := project.show_prerequisite(?,?); end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            BufferedReader  readKeyBoard;
            String       dcode;
	    String       Cnumber;
	    //enter claasid and dept_code
            readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("Please enter dept_code: ");
            dcode = readKeyBoard.readLine();
            cs.setString(2, dcode);
            System.out.print("Please enter course#: ");
            Cnumber = readKeyBoard.readLine();
            if(isNumber(Cnumber)){
                cs.setInt(3, Integer.parseInt(Cnumber));
                cs.execute();
                ResultSet rs = (ResultSet)cs.getObject(1);
                
                if(rs.next()){
                    System.out.println("dept_code	" + "course#");
                    System.out.println("");
                    System.out.println(rs.getString(1) + "\t" + "\t" + rs.getString(2));
                    while (rs.next()) {
                        System.out.println(rs.getString(1) + "\t" + "\t" + rs.getString(2));
                    }
                }else{
                    System.out.println("dept_code || course# does not exist.");
                }
            }else{
                System.out.println("dept_code || course# does not exist.");
            }
            cs.close();
        }
        catch (SQLException ex) { System.out.println ("\n*** SQLException caught ***\n" + ex.getMessage());}
        catch (Exception e) {
        	System.out.println("\n*** other Exception caught ***\n");
        }
    }

    public static void enrollstudent() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement();

            stmt.executeUpdate("begin dbms_output.enable(); end;");
	    //use project.enroll() function in plsql
            CallableStatement cs = conn.prepareCall("begin project.enroll(?, ?); end;");

            BufferedReader readKeyBoard;
            String Bnumber;
            String classid;
            readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
            //enter B# and classid
            System.out.print("Please enter B#: ");
            Bnumber = readKeyBoard.readLine();
            cs.setString(1, Bnumber);
            System.out.print("Please enter ClassID: ");
            classid = readKeyBoard.readLine();
            cs.setString(2, classid);

            cs.execute();
	    //get dbms_output.put_line() message in plsql
            try (CallableStatement call = conn.prepareCall(
                    "declare "
                            + "  num integer := 1000;"
                            + "begin "
                            + "  dbms_output.get_lines(?, num);"
                            + "end;")) {
                call.registerOutParameter(1, Types.ARRAY,
                        "DBMSOUTPUT_LINESARRAY");
                call.execute();
                Array array = null;
                try {
                    array = call.getArray(1);
                    if (((Object[]) array.getArray()).length != 0) {
                        Stream.of(((Object[]) array.getArray())[0]).forEach(System.out::println);
                    }
                } finally {
                    if (array != null)
                        array.free();
                }
                call.close();
                cs.close();
            }
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }

    public static void dropstudent() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement();

            stmt.executeUpdate("begin dbms_output.enable(); end;");
	    //use project.drop_class() function in plsql
            CallableStatement cs = conn.prepareCall("begin project.drop_class(?, ?); end;");

            BufferedReader readKeyBoard;
            String Bnumber;
            String classid;
            readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
            //enter B# and classid
            System.out.print("Please enter B#: ");
            Bnumber = readKeyBoard.readLine();
            cs.setString(1, Bnumber);
            System.out.print("Please enter ClassID: ");
            classid = readKeyBoard.readLine();
            cs.setString(2, classid);

            cs.execute();
	    //get dbms_output.put_line() message in plsql
            try (CallableStatement call = conn.prepareCall(
                    "declare "
                            + "  num integer := 1000;"
                            + "begin "
                            + "  dbms_output.get_lines(?, num);"
                            + "end;")) {
                call.registerOutParameter(1, Types.ARRAY,
                        "DBMSOUTPUT_LINESARRAY");
                call.execute();
                Array array = null;
                try {
                    array = call.getArray(1);
                    if (((Object[]) array.getArray()).length != 0) {
                        Stream.of(((Object[]) array.getArray())[0]).forEach(System.out::println);
                    }
                } finally {
                    if (array != null)
                        array.free();
                }
                call.close();
                cs.close();
            }
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }

    public static void deletestudent() throws SQLException {
        try {
            // OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            // ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            // Connection conn = ds.getConnection("jxia27", "600322");

            // Statement stmt = conn.createStatement ();

            stmt.executeUpdate("begin dbms_output.enable(); end;");
	    //use project.delete_students() function in plsql
            CallableStatement cs = conn.prepareCall("begin project.delete_students(?); end;");

            BufferedReader readKeyBoard;
            String Bnumber;
            readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
            //enter B#
            System.out.print("Please enter B#: ");
            Bnumber = readKeyBoard.readLine();
            cs.setString(1, Bnumber);

            cs.execute();
    	    //get dbms_output.put_line() message in plsql
            try (CallableStatement call = conn.prepareCall(
                    "declare "
                            + "  num integer := 1000;"
                            + "begin "
                            + "  dbms_output.get_lines(?, num);"
                            + "end;")) {
                call.registerOutParameter(1, Types.ARRAY,
                        "DBMSOUTPUT_LINESARRAY");
                call.execute();

                Array array = null;
                try {
                    array = call.getArray(1);
                    if (((Object[]) array.getArray()).length != 0) {
                        Stream.of(((Object[]) array.getArray())[0]).forEach(System.out::println);
                    }
                } finally {
                    if (array != null)
                        array.free();
                }
                call.close();
                cs.close();
            }
        } catch (SQLException ex) {
            System.out.println("\n*** SQLException caught ***\n" + ex.getMessage());
        } catch (Exception e) {
            System.out.println("\n*** other Exception caught ***\n");
        }
    }
}
