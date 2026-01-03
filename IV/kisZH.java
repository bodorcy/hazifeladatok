import java.util.ArrayList;
import java.util.concurrent.Semaphore;

class ParkingLot{
    private final int CAPACITY = 10;
    ArrayList<Integer> cars;
    Semaphore semaphore;
    public ParkingLot(){
        this.cars = new ArrayList<>(CAPACITY);
        this.semaphore = new Semaphore(CAPACITY);
    }

    public void enter(Car car){
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        synchronized (this){
            cars.add(car.num);
            System.out.println(cars);
        }
    }
    public void leave(){
        synchronized (this){
            if(!cars.isEmpty()) {
                cars.remove(0);
            }
        }
        semaphore.release();
    }
    public void parking(){
        try {
            Thread.sleep(300);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
class Car extends Thread{
    public int num;
    public ParkingLot parkingLot;
    public Car(int carnum, ParkingLot parkingLot){
        this.num = carnum;
        this.parkingLot = parkingLot;
    }
    @Override
    public void run(){
        parkingLot.enter(this);
        parkingLot.parking();
        parkingLot.leave();
    }
}

public class kisZH{
    public static void main(String[] args) {
        int numOfCars = 100;

        ParkingLot parkingLot = new ParkingLot();

        for(int i = 0; i < numOfCars; ++i){
            Car car = new Car(i + 1, parkingLot);
            car.start();
        }
    }
}
