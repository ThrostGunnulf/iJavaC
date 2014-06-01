/*Programas mais complexos*/
class test29
{
	static boolean[] isNegative;

	public static void main(String[] args)
	{
		int i, sum, fib;
		int[] parsedArgs, aux;
		
		i = 0;
		parsedArgs = new int[args.length];
		while(i < args.length)
		{
			parsedArgs[i] = Integer.parseInt(args[i]);
			i = i + 1;
		}
		
		aux = change(parsedArgs);
		sum = sum(aux);
		fib = fibonacci(sum);
		
		System.out.println(fib);
		
	}
	
	public static int[] change(int[] array)
	{
		int i;
		
		i = 0;
		isNegative = new boolean[array.length];
		while(i < array.length)
		{
			if(array[i] < 0)
			{
				isNegative[i] = true;
				array[i] = array[i] + 1;
			}
			else if(array[i] > 0)
			{
				isNegative[i] = false;
				array[i] = array[i] - 1;
			}
			
			i = i + 1;
		}
		
		return array;
	}
	
	public static int sum(int[] array)
	{
		int i, sum;
		
		i = 0; sum = 0;
		while(i < array.length)
		{
			sum = sum + array[i];
			i = i + 1;
		}
		
		return sum;
	}
	
	public static int fibonacci(int n)
	{
		if(n <= 1)
			return 1;
		else
			return fibonacci(n -1) + fibonacci(n -2);
	}
}
