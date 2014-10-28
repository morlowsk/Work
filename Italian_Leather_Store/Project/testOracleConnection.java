import java.sql.*;

public class testOracleConnection {
	public static void main (String [] args) throws SQLException {
	
	try {
		Class.forName("oracle.jdbc.driver.OracleDriver");
	}
	catch (ClassNotFoundException e) { return null; } 
	
	try {
		Connection conn = DriverManager.getConnection("jdbc:oracle:thin:@fourier.cs.iit.edu", "morlowsk", "weaselthorptlahuac1");
		boolean reachable = conn.isValid(10);
		System.out.println(reachable);
	}
	catch (SQLException BLEGH) {
		BLEGH.printStackTrace();
	}
	}
}
