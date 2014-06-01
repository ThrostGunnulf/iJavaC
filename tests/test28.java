/*Atribuição de valores a posições de arrays*/
class test28
{
	static int[] a1;
	static boolean[] b1;
		
	public static void main(String[] args)
	{
		int[] a2;
		boolean[] b2;
		
		a1 = new int[3];
		a1[0] = -1;
		a1[1] = 0x0;
		a1[2] = 01;
		
		a2 = new int[3];
		a2[0] = -2;
		a2[1] = 0x0;
		a2[2] = 02;
		
		b1 = new boolean[4];
		b1[0] = false;
		b1[1] = true;
		b1[2] = false;
		b1[3] = true;
		
		b2 = new boolean[4];
		b2[0] = false;
		b2[1] = true;
		b2[2] = false;
		b2[3] = true;
		
		System.out.println(a1[0]);
		System.out.println(a1[1]);
		System.out.println(a1[2]);
		
		System.out.println(a2[0]);
		System.out.println(a2[1]);
		System.out.println(a2[2]);
		
		System.out.println(b1[0]);
		System.out.println(b1[1]);
		System.out.println(b1[2]);
		System.out.println(b1[3]);
		
		System.out.println(b2[0]);
		System.out.println(b2[1]);
		System.out.println(b2[2]);
		System.out.println(b2[3]);
	}
}
