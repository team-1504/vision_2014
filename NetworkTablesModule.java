import edu.wpi.first.wpilibj.networktables.NetworkTable;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.lang.Thread;

public class NetworkTablesModule
{
	private static Object[] packet_out;

	private static NetworkTable robot_table;
	private static NetworkTable inform_table;
	private static NetworkTable cmd_table;

	private static int mode;
	private static double dx;
	private static double dy;
	private static double theta;

	public static void initialize_packet(int size)
	{
		packet_out = new Object[size];
	}
	
	public static void add_int(int value, int index)
	{
//		System.out.println("Adding int: " + value);
		packet_out[index] = new Integer(value);
	}
	
	public static void add_bool(boolean val, int index)
	{
//		System.out.println("Adding bool: " + val);
		packet_out[index] = new Boolean(val);
	}

	public static void add_double(double val, int index)
	{
//		System.out.println("Adding double: " + val);
		packet_out[index] = new Double(val);
	}
	public static int get_mode()
	{
		return mode;
	}

	public static double get_dx()
	{
		return dx;
	}

	public static double get_dy()
	{
		return dy;
	}

	public static double get_angle()
	{
		return theta;
	}

	public static void init(String ip)
	{
		System.out.println("Initiliazing java module.\n");
		NetworkTable.setClientMode();
		System.out.println("Client mode set.\n");
		NetworkTable.setIPAddress(ip);
		System.out.println("IP set; getting tables\n");
		robot_table = NetworkTable.getTable("robot_table");
		inform_table = NetworkTable.getTable("inform_table");
		cmd_table = NetworkTable.getTable("cmd_table");
		System.out.println("Tables got.\n");
	}

	public static void run()
	{
//		System.out.println("Running.");	

		if (packet_out.length > 0)
		{
//			System.out.println("Setting out packet.");	
			int num_targets = ((Integer)packet_out[0]).intValue();
			inform_table.putNumber("num_targets", num_targets);
			for (int i = 0; i < num_targets; i+=3)
			{
				inform_table.putNumber("target_"+i+"_heading", ((Double)packet_out[1+3*i]).doubleValue());
				inform_table.putNumber("target_"+i+"_distance", ((Double)packet_out[1+3*i+1]).doubleValue());
				inform_table.putNumber("target_"+i+"_type", ((Integer)packet_out[1+3*i+2]).intValue());
			}
		}

		mode = (int)robot_table.getNumber("game_mode", 1.0);
		System.out.println("mode: " + mode);
		dx = robot_table.getNumber("robot_dx", 150.0);
		dy = robot_table.getNumber("robot_dy", 0);
		theta = robot_table.getNumber("compass_heading", 0);

		int num = (int)inform_table.getNumber("num_targets", 0.0);
		System.out.println("Num_targets: " + num);
	}
}
