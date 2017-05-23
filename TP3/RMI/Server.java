package tp3.rmi;

import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;

public class Server implements Interface {

    public Server() {}

	@Override
	public double[] rmi_log(double[] vec, int b) {
		double[] ret = new double[vec.length];
		for (int i=0; i<vec.length; i++) {
			ret[i] = Math.log(vec[i])/Math.log(b);
		}
		return ret;
	}
	
	@Override
	public int rmi_count_greater(double[] vec, int n) {
		int ret = 0;
		for (int i=0; i<vec.length; i++) {
			if (vec[i]>n) {
				ret+=1;
			}
		}
		return ret;
	}
	
	@Override
	public double[] rmi_nth_root(double[] vec, int n) {
		double[] ret = new double[vec.length];
		double exp = 1.0/n;
		for (int i=0; i<vec.length; i++) {
			ret[i] = Math.pow(vec[i],exp);
		}
		return ret;
	}
	
	@Override
	public double[] rmi_mult(double[] vec, double n) {
		double[] ret = new double[vec.length];
		for (int i=0; i<vec.length; i++) {
			ret[i]=vec[i]*n;
		}
		return ret;
	}
	
	@Override
	public int rmi_count_multiples(double[] vec, int n) {
		int ret = 0;
		for (int i=0; i<vec.length; i++) {
			if (vec[i]%n==0) {
				ret+=1;
			}
		}
		return ret;
	}
	
	@Override
	public int rmi_count_powers(double[] vec, int n) {
		int ret = 0;
		int element=0;
		double test=0;
		int intTest=0;
		for (int i=0; i<vec.length; i++) {
			element = (int)vec[i];
			test = Math.log(element)/Math.log(n);
			intTest = (int)test;
			if (test-intTest==0) {
				ret+=1;
			}
		}
		return ret; 
	}

    public static void main(String args[]) {

        try {
            Server obj = new Server();
            Interface stub = (Interface) UnicastRemoteObject.exportObject(obj, 0);

            Registry registry = LocateRegistry.getRegistry();
            registry.bind("Hello", stub);

            System.err.println("Server ready");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
