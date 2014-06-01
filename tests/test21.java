/*Short circuiting*/
class test21
{
	public static void main(String[] args)
	{
		int a, b;
		boolean c, d;
		
		c = true && test1();
		d = false && test1();
		
		c = true && test2();
		d = false && test2();
		
		c = false && test1();
		d = true && test1();
		
		c = false && test2();
		d = true && test2();
	}
	
	public static boolean test1()
	{
		System.out.println(1);
		
		return true;
	}
	
	public static boolean test2()
	{
		System.out.println(2);
		
		return false;
	}
}
