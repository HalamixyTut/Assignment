// usage:  1. compile: javac -cp /usr/lib/oracle/18.3/client64/lib/ojdbc8.jar jdbcdemo2.java
//         2. execute: java -cp /usr/lib/oracle/18.3/client64/lib/ojdbc8.jar jdbcdemo2.java
//
//Illustrate call stored procedure
import java.sql.*;
import oracle.jdbc.*;
import java.math.*;
import java.io.*;
import java.awt.*;
import oracle.jdbc.pool.OracleDataSource;

public class jdbcdemo2 {

    public static void main (String args []) throws SQLException {
       	try
	{

	  //Connection to Oracle server. Need to replace username and
	  //password by your username and your password. For security
	  //consideration, it's better to read them in from keyboard.
	  OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
	  ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:ACAD111");
	  Connection conn = ds.getConnection("username", "password");

	  // Input sid from keyboard
	  BufferedReader  readKeyBoard;
	  String          sid;
	  readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
	  System.out.print("Please Enter SID:");
	  sid = readKeyBoard.readLine();

	  //Prepare to call stored procedure:
	    // create or replace procedure show_status (sid_in in students2.sid%type,
	    //                     status_out out students2.status%type) is
            // begin select status into status_out from students2 where sid = sid_in;
            // end;
	  CallableStatement cs = conn.prepareCall("begin show_status(:1,:2); end;");

	  //set the in parameter (the first parameter)
	  cs.setString(1, sid);

	  //register the out parameter (the second parameter)
	  cs.registerOutParameter(2, Types.VARCHAR);

	  //execute the stored procedure
	  cs.executeQuery();

	  //get the out parameter result.
	  String status = cs.getString(2);
	  System.out.println(sid+" : "+status);

	  //close the result set, statement, and the connection
	  cs.close();
	  conn.close();
     }
     catch (SQLException ex) { System.out.println ("\n*** SQLException caught ***\n" + ex.getMessage());}
     catch (Exception e) {System.out.println ("\n*** other Exception caught ***\n");}
  }
}

