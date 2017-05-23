package tp3.rmi;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Random;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Client {

	public static int num_threads;
	public static double[] vec;
	public static String host;
	public static int func;
	public static int result;
	public static Lock lock = new ReentrantLock();
	public static double arg;
	
    private Client() {}
	

    public static void main(String[] args) {

		host = (args.length < 5) ? null : args[4];
		num_threads = (args.length < 1) ? 1 : Integer.parseInt(args[0]);
		int vecsize = (args.length < 2) ? 10000000 : Integer.parseInt(args[1]);
		func = (args.length < 3) ? 1 : Integer.parseInt(args[2]);
		arg = (args.length < 4) ? 100 : Double.parseDouble(args[3]);
		Thread[] threads = new Thread[num_threads];
		
		vec = new double[vecsize];
		Random random = new Random();
		for (int i=0; i<vec.length; i++) {
			vec[i] = (double)random.nextInt(32768);
		}
		long itime = System.currentTimeMillis();
		for (int i=0; i<num_threads; i++) {
			switch(func) {
				case 1:
				case 2:
				case 3:
					threads[i] = new Thread1(i);
					break;
				case 4:
				case 5:
				case 6:
					threads[i] = new Thread2(i);
					break;
			}
			threads[i].start();
		}
		for (int i=0; i<num_threads; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		//if (func>3) {
		//	System.out.println("Resultado: "+result);
		//}
		long ftime = System.currentTimeMillis();
		double ttime = (double)(ftime-itime)/1000;
		System.out.println("Tempo: "+ttime+" segundos.");		
    }
}

class Thread1 extends Thread {
	private int tid;
	public Thread1(int tid) {
		this.tid = tid;
	}
	@Override
	public void run() {
		int iindex = tid * (Client.vec.length/Client.num_threads);
		int findex = (tid < Client.num_threads-1) ? (tid+1)*(Client.vec.length/Client.num_threads) : Client.vec.length;
		double[] t_vec = new double[findex-iindex];
		for (int i=iindex; i < findex; i++) {
			t_vec[i-iindex] = Client.vec[i];
		}
		try {
			Registry registry = LocateRegistry.getRegistry(Client.host);
			Interface stub = (Interface) registry.lookup("Hello");
			switch(Client.func) {
				case 1:
					t_vec=stub.rmi_log(t_vec, (int)Client.arg);
					break;
				case 2:
					t_vec=stub.rmi_nth_root(t_vec, (int)Client.arg);
					break;
				case 3:
					t_vec=stub.rmi_mult(t_vec, Client.arg);
					break;
			}
		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
		}
		for (int i=iindex; i < findex; i++) {
			Client.vec[i] = t_vec[i-iindex];
		}
	}
}

class Thread2 extends Thread {
	private int tid;
	public Thread2(int tid) {
		this.tid = tid;
	}
	@Override
	public void run() {
		int iindex = tid * (Client.vec.length/Client.num_threads);
		int findex = (tid < Client.num_threads-1) ? (tid+1)*(Client.vec.length/Client.num_threads) : Client.vec.length;
		int result = 0;
		double[] t_vec = new double[findex-iindex];
		for (int i=iindex; i < findex; i++) {
			t_vec[i-iindex] = Client.vec[i];
		}
		try {
			Registry registry = LocateRegistry.getRegistry(Client.host);
			Interface stub = (Interface) registry.lookup("Hello");
			switch(Client.func) {
				case 4:
					result=stub.rmi_count_greater(t_vec, (int)Client.arg);
					break;
				case 5:
					result=stub.rmi_count_multiples(t_vec, (int)Client.arg);
					break;
				case 6:
					result=stub.rmi_count_powers(t_vec, (int)Client.arg);
					break;
			}
		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
		}
		Client.lock.lock();
		Client.result+=result;
		Client.lock.unlock();
	}
}

