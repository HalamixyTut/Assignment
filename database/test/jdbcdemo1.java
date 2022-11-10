// usage:  1. compile: javac -cp /usr/lib/oracle/18.3/client64/lib/ojdbc8.jar jdbcdemo1.java
//         2. execute: java -cp /usr/lib/oracle/18.3/client64/lib/ojdbc8.jar jdbcdemo1.java

import java.sql.*;
import oracle.jdbc.*;
import java.math.*;
import java.io.*;
import java.awt.*;
import oracle.jdbc.pool.OracleDataSource;

public class jdbcdemo1 {

     public static void main (String args []) throws SQLException {
       try
       {

        //Connection to Oracle server. Need to replace username and
        //password by your username and your password. For security
        //consideration, it's better to read them in from keyboard.
	OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
	ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
	Connection conn = ds.getConnection("yli500", "Lyq19960425");

        // Query
	Statement stmt = conn.createStatement ();

        // Save result
	ResultSet rset;
	rset = stmt.executeQuery ("SELECT * FROM students");

        // Print
	while (rset.next ()) {
	   System.out.print (rset.getString (1)+"  ");
	   System.out.print (rset.getString (2)+"  ");
	   System.out.print (rset.getString (3)+"  ");
	   System.out.print (rset.getString (4)+"  ");
	   System.out.print (rset.getString (5)+"  ");
	   System.out.println (rset.getString (6)+"  ");
	}

        //update
	String updateString = "UPDATE students2 SET status = 'senior' WHERE sid = 'B001'";
        stmt.executeUpdate(updateString);

	//Insert
	PreparedStatement insert = conn.prepareStatement("INSERT into students2 VALUES(?,?,?,?,?,?)");

	//Input sid from keyboard
	BufferedReader  readKeyBoard;
	String          sid;
	readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
	System.out.print("Please enter SID:");
	sid = readKeyBoard.readLine();
	insert.setString(1, sid);

	//Input other values
	insert.setString(2, "Mark");
	insert.setString(3, "Twin");
	insert.setString(4, "freshman");
	insert.setDouble(5, 3.5);
	insert.setString(6, "mtwin2@bu.edu");

	// execute the update
	insert.executeUpdate();

	//Query again.
	rset = stmt.executeQuery ("SELECT * FROM students2");

	// Print
	while (rset.next ()) {
	   System.out.print (rset.getString (1)+"  ");
	   System.out.print (rset.getString (2)+"  ");
	   System.out.print (rset.getString (3)+"  ");
	   System.out.print (rset.getString (4)+"  ");
	   System.out.print (rset.getString (5)+"  ");
	   System.out.println (rset.getString (6)+"  ");
	}

	//close the result set, statement, and the connection
	rset.close();
	stmt.close();
	conn.close();
    }
	catch (SQLException ex) { System.out.println ("\n*** SQLException caught ***\n");}
	catch (Exception e) {System.out.println ("\n*** other Exception caught ***\n");}
  }
}

