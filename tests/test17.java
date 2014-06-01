/*Ifs*/
class test17
{
	public static void main(String[] args)
	{
		boolean b1, b2;
		
		b1 = true; b2 = false;
	
		if(b1)
			System.out.println(0);
		
		if(b2)
			System.out.println(1);
			
		if(b1)
			System.out.println(2);
		else
			System.out.println(3);
			
		if(b2)
			System.out.println(4);
		else
			System.out.println(5);
			
		if(b1) {}
		
		if(b2) {}
		
		if(b1) {}
		else {}
		
		if(b2) {}
		else {}
		
		if(b1)
			System.out.println(6);
		else {}
		
		if(b2)
			System.out.println(7);
		else {}
		
		if(b1) {}
		else 
			System.out.println(8);
		
		if(b2) {}
		else 
			System.out.println(9);
	}
}
