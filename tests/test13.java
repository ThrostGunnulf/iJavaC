/*Operadores unários aplicados a literais*/
class test13
{
	public static void main(String[] args)
	{
		System.out.println(-(-1));
		System.out.println(-(1));
		System.out.println(-(0));
		System.out.println(+(-1));
		System.out.println(+(1));
		System.out.println(+(0));
		System.out.println(-(-01));
		System.out.println(-(01));
		System.out.println(-(00));
		System.out.println(+(-01));
		System.out.println(+(01));
		System.out.println(+(00));
		System.out.println(-(-0x1));
		System.out.println(-(0x1));
		System.out.println(-(0x0));
		System.out.println(+(-0x1));
		System.out.println(+(0x1));
		System.out.println(+(0x0));
		System.out.println(!false);
		System.out.println(!true);
	}
}
