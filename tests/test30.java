/*Fluxo de controlo*/
class test30
{
	static int i;
	static boolean b;

	public static void main(String[] args)
	{
		int j;
		
		i = 0;
		while(i <= 100)
		{
			if(i > 10 && test1())
				{}
			
			j = 0;
			while(j <= 10) {j = j + 1;}
			
			if(!b)
			{}
			else
			{
				System.out.println(args.length);
			}
			
			i = i + 1;
		}
	}
	
	public static boolean test1()
	{
		System.out.println(i);
	
		if(i >=  50)
		{
			b = true;
			return true;
		}
		else
		{
			b = false;
			return false;
		}
	}
}
