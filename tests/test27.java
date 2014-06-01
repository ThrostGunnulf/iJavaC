/*Indexação de arrays alocados mas não inicializados*/
class test27
{
	static int[] a1;
	static boolean[] b1;
		
	public static void main(String[] args)
	{
		int[] a2;
		boolean[] b2;
		
		a1 = new int[3];
		a2 = new int[3];
		
		b1 = new boolean[4];
		b2 = new boolean[4];
		
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
