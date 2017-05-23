package tp3.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;

public interface Interface extends Remote {
    double[] rmi_log(double[] vec, int b) throws RemoteException;
	double[] rmi_nth_root(double[] vec, int n) throws RemoteException;
	double[] rmi_mult(double[] vec, double n) throws RemoteException;
	int rmi_count_greater(double[] vec, int n) throws RemoteException;
	int rmi_count_multiples(double[] vec, int n) throws RemoteException;
	int rmi_count_powers(double[] vec, int n) throws RemoteException;
}
