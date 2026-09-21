// Document with my answers to whether program compiles or not.
// I think this doesnt compile be
// Q1
// fn main() {
//     let mut s = String::from("hello");
//     let ref1 = &s;
//     let ref2 = &ref1;
//     let ref3 = &ref2;
//     s = String::from("goodbye");
//     println!("{}", ref3.to_uppercase());
// }

// 
// 
// 
// 2 drip_drop apparently creates a locally owned string s and returns a reference to it from the function.
// this function should compile
fn drip_drop() -> &String {
    let s = String::from("hello world!");
    return &s;
}
// 3 Should we be able to move v[0] out of the array when v is not needed any more in the function?
// 
fn main() {
    let s1 = String::from("hello");
    let mut v = Vec::new();
    v.push(s1);
    let s2: String = v[0];
    println!("{}", s2);
}
