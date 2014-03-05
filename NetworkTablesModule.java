public class NetworkTablesModule
{
	public static Object[] get_packet()
	{
		Object[] packet = new Object[1];
		packet[0] = new Integer(10);
		return packet;
	}

	public static void test_print()
	{
		System.out.println("Hello World from Java!");
	}
}
