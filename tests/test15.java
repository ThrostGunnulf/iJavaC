/*Operadores binários aplicados a literais*/
class test15
{
	public static void main(String[] args)
	{
		//1 - 4
		System.out.println(1+0xF);
		System.out.println(0xF-(-1));
		System.out.println(-07+1);
		System.out.println(-33-7);
		
		//5 - 11
		System.out.println(1*13);
		System.out.println(0x1*0xD);
		System.out.println(01*7);
		System.out.println(7*3);
		System.out.println(8*(-2));
		System.out.println((-4)*02);
		System.out.println((-3)*(0x3));
		
		//12 - 19
		System.out.println(3/3);
		System.out.println(3/2);
		System.out.println(3/4);
		System.out.println(-3/3);
		System.out.println(3/(-2));
		System.out.println(3/0x4);
		System.out.println(05/07);
		System.out.println(100/50);
		
		//20 - 27
		System.out.println(3%3);
		System.out.println(3%2);
		System.out.println(3%4);
		System.out.println(-3%3);
		System.out.println(3%(-2));
		System.out.println(3%0x4);
		System.out.println(05%07);
		System.out.println(100%50);
		
		//28 - 33
		System.out.println(03 < 2);
		System.out.println(2 < 3);
		System.out.println(1 < 0x1);
		System.out.println(-3 < 02);
		System.out.println(-0x2 < -3);
		System.out.println(-3 < -3);
		
		//34 - 39
		System.out.println(0x3 <= 2);
		System.out.println(2 <= 03);
		System.out.println(1 <= 1);
		System.out.println(-3 <= 0x2);
		System.out.println(-02 <= -03);
		System.out.println(-0x3 <= -3);
		
		//40 - 45
		System.out.println(3 > 02);
		System.out.println(2 > 0x3);
		System.out.println(0x1 > 0x1);
		System.out.println(-3 > 2);
		System.out.println(-02 > -3);
		System.out.println(-0x3 > -3);
		
		//46 - 51
		System.out.println(0x3 >= 2);
		System.out.println(2 >= 3);
		System.out.println(1 >= 01);
		System.out.println(-03 >= 2);
		System.out.println(-2 >= -0x3);
		System.out.println(-03 >= -0x3);
		
		//52 - 57
		System.out.println(3 != 2);
		System.out.println(02 != 3);
		System.out.println(01 != 1);
		System.out.println(-0x3 != 2);
		System.out.println(-2 != -0x3);
		System.out.println(-3 != -03);
		
		//58 - 63
		System.out.println(3 == 0x2);
		System.out.println(2 == 03);
		System.out.println(0x1 == 1);
		System.out.println(-03 == 2);
		System.out.println(-2 == -3);
		System.out.println(-03 == -0x3);
		
		//64 - 67
		System.out.println(true && true);
		System.out.println(true && false);
		System.out.println(false && true);
		System.out.println(false && false);
		
		//68 - 71
		System.out.println(true || true);
		System.out.println(true || false);
		System.out.println(false || true);
		System.out.println(false || false);
	}
}
