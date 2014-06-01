/*Chamadas a funções*/
class test20
{
	public static void main(String[] args)
	{
		int a, b;
		int[] a2, b2;
		boolean c, d;
		boolean[] c2, d2;
		
		a = test1();
		b = test2(a, -1);
		c = test3();
		d = test4();
		
		a2 = test5();
		b2 = test6();
		c2 = test7();
		d2 = test8();
		
		System.out.println(a);
		System.out.println(b);
		System.out.println(c);
		System.out.println(d);
		System.out.println(b2[0]);
		System.out.println(d2[0]);
		System.out.println(d2[1]);
	}
	
	public static int test1()
	{
		return 1;
	}
	
	public static int test2(int a, int b)
	{
		return a + b;
	}
	
	public static boolean test3()
	{
		return true;
	}
	
	public static boolean test4()
	{
		return false;
	}
	
	public static int[] test5()
	{
		return new int[2];
	}
	
	public static int[] test6()
	{
		int[] b;
		b = new int[1];
		b[0] = 0x7F;
		
		return b;
	}
	
	public static boolean[] test7()
	{
		return new boolean[0];
	}
	
	public static boolean[] test8()
	{
		boolean[] a;
		a = new boolean[7];
		a[0] = false;
		a[1] = true;
		
		return a;
	}
}
