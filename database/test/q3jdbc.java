import java.sql.*;
import oracle.jdbc.*;
import java.math.*;
import java.io.*;
import java.awt.*;
import oracle.jdbc.pool.OracleDataSource;

public class q3jdbc {
    public static void main (String args []) throws SQLException {
        try {
            OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();
            ds.setURL("jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:acad111");
            Connection conn = ds.getConnection("yli500", "Lyq19960425");

            Statement stmt = conn.createStatement ();

            CallableStatement cs = conn.prepareCall("begin ? := project.showStudentsByClassid(?); end;");
            cs.registerOutParameter(1, OracleTypes.CURSOR);

            BufferedReader  readKeyBoard;
            String          classid;
            readKeyBoard = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("Please enter ClassID: ");
            classid = readKeyBoard.readLine();
            cs.setString(2, classid);

            cs.execute();
            ResultSet rs = (ResultSet)cs.getObject(1);

            while (rs.next()) {
                System.out.println(rs.getString(1) + "\t" +
                        rs.getString(2) + "\t" + rs.getString(3));
            }
            cs.close();
            conn.close();
        }
        catch (SQLException ex) { System.out.println ("\n*** SQLException caught ***\n" + ex.getMessage());}
        catch (Exception e) {System.out.println ("\n*** other Exception caught ***\n");
        }
    }
}
