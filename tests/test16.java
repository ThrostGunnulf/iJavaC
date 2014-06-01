/*Whiles*/
class test16
{
	public static void main(String[] args)
	{
		boolean b;
		int i, r, lim;
		
		i = 1; r = 0;
		lim = Integer.parseInt(args[0]);
		
		while(i <= lim)
		{
			r = r + i;
			i = i + 1;
		}
		
		System.out.println(r);
		
		b = false;
		while(b) {}
	}
}
