/*Expressões random*/
class test34
{
	public static void main(String[] args)
	{
		int a;
		boolean b, b11, b12;
		boolean[] b2;
		
		a = (new int[1])[0];
		System.out.println(a);
		
		b2 = new boolean[2];
		b2[0] = true; b2[1] = false;
		b11 = false; b12 = false;
		b = b12 && (b11 || b12);
		System.out.println(b);
		b = b2[0] && (b2[1] || (test1() && (b2[1] && b2[0])));
		System.out.println(b);
	}
	
	public static boolean test1()
	{
		boolean b1, b2;
		b1 = true; b2 = false;
		
		return b1 || b2 && b1 && b2;
	}
}
