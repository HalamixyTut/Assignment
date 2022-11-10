import java.sql.*;
import oracle.jdbc.*;
import java.math.*;
import java.io.*;
import java.awt.*;
import oracle.jdbc.pool.OracleDataSource;
import java.util.regex.Pattern;
import java.util.stream.Stream;

public class q4jdbc {

    public static boolean isNumber(String str) {
	Pattern pattern = Pattern.compile("[0-9]*");
	return pattern.matcher(str).matches();
    }    

    public static void main (String args []) throws SQLException {
        try {
            OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            Connection conn = ds.getConnection("yli500", "Lyq19960425");

            Statement stmt = conn.createStatement ();
            CallableStatement cs = conn.prepareCall("begin ? := project.show_prerequisite(?,?); end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            BufferedReader  readKeyBoard;
            String       dcode;
	        String       Cnumber;
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
        }
        catch (SQLException ex) { System.out.println ("\n*** SQLException caught ***\n" + ex.getMessage());}
        catch (Exception e) {}
    }
}
