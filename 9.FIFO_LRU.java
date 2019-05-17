package ssos;

import java.util.Scanner;

public class Main {

    static Scanner sc = new Scanner(System.in);
    static int pageRequests[],frame[],frameSize,noOfPageRequests;
    static int hit=0,marker=0;
    
    public static void main(String []args) {
    	
        System.out.println("Select page replacement algorithm\n");
        
        System.out.println("Enter the frame size\n");
        frameSize = sc.nextInt();
       
        System.out.println("Enter no of page requests\n");
        noOfPageRequests = sc.nextInt();
       
        frame = new int[frameSize];
        pageRequests = new int[noOfPageRequests];
       
        System.out.println("Enter the requests \n");
        for(int i=0;i<noOfPageRequests;i++)
            pageRequests[i] = sc.nextInt();
        
        System.out.println("1)FIFO 2)LRU\n");
        switch(sc.nextInt()) {
        case 1:runFIFO();
            break;
        case 2:runLRU();
            break;
        }
    }

    static void runFIFO() {
        
        for(int i=0;i<noOfPageRequests;i++) {
            displayFrame(frame);
            if(i<frameSize)
                frame[i] = pageRequests[i];
            else {
                boolean check_hit = checkForHit(frame,pageRequests[i]);
                if(check_hit)
                    hit++;
                else {
                    frame[marker] = pageRequests[i];
                    marker = (marker+1)%3;
                }
            }
        }
        System.out.println("Page hit = "+hit);
        System.out.println("Page fault = "+(noOfPageRequests-hit));
     }
    
    static void runLRU() {
        for(int i=0;i<noOfPageRequests;i++) {
        	
        	displayFrame(frame);
            if(i<frameSize)
            	pushToStack(pageRequests[i]);
            else {
                boolean check_hit = checkForHit(frame,pageRequests[i]);
                if(check_hit) {
                    hit++;
                    pushToStack(pageRequests[i]);
                }
                else {
                    pushToStack(pageRequests[i]);
                }
            }
        }
        System.out.println("Page hit = "+hit);
        System.out.println("Page fault = "+(noOfPageRequests-hit));
    }

	static void displayFrame(int[] frame) {
        String stack = "[";
        for(int i=0;i<frame.length;i++) {
        	stack += " "+frame[i];
        }
        stack += " ]";
        System.out.println(stack);
    }

    static boolean checkForHit(int []frame,int num) {
        for(int i=0;i<frame.length;i++)
            if(frame[i]==num)return true;
        return false;
    }

    static void pushToStack(int num) {
    	int []tempArray= new int[frameSize];
    	tempArray[0] = num;
    	
    	for(int i=1,j=0;i<frameSize && j<frameSize;j++){
    		if(frame[j]!=num){
    			tempArray[i] = frame[j];
    			i++;
    		}
    	}
       frame = tempArray;
    	
    }
}
