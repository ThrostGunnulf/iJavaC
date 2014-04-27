class gcd {
	public static void main(String[] args) {
		int[] x;
		x = new int[2];
		x[0] = Integer.parseInt(args[0]);
		x[1] = Integer.parseInt(args[1]);
		if (x[0] == 0)
			System.out.println(x[1]);
		else
			while (x[1] > 0) {
				if (x[0] > x[1])
					x[0] = x[0] - x[1];
				else
					x[1] = x[1] - x[0];
			}
		System.out.println(x[0]);
	}
}
