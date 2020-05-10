#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn square(width: u32) -> Rectangle {
        Rectangle {
            width: width,
            height: width,
        }
    }

    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn double(&mut self) {
        self.width *= 2;
        self.height *= 2;
    }

    fn contains(&self, other: &Rectangle) -> bool {
        (self.width >= other.width) && (self.height >= other.height)
    }
}

fn main() {
    let mut rect1 = Rectangle {
        width: 30,
        height: 50,
    };

    println!("{:?}", rect1);
    println!("{:#?}", rect1);

    println!("{}", rect1.area());
    let rect2 = Rectangle { ..rect1 };
    rect1.double();
    println!("{}", rect1.area());
    println!("{}", rect1.contains(&rect2));
    println!("{}", rect1.contains(&Rectangle::square(10)));
}
