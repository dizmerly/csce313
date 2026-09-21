// 1 What is the output of this program?

// It doesnt compile becuase ref3 is a reference to s,
// and when s is reassigned, ref still clings to old string,
// which is now dropped
fn main() {
    let mut s = String::from("hello");
    let ref1 = &s;
    let ref2 = &ref1;
    let ref3 = &ref2;
    s = String::from("goodbye");
    println!("{}", ref3.to_uppercase());
}
// // // 2 drip_drop apparently creates a locally owned string s and returns a reference to it from the function.

// // This doesnt compile because you cant pass a reference to a variable that will be dropped
fn drip_drop() -> &String {
    let s = String::from("hello world!");
    return &s;
}

// // 3 Should we be able to move v[0] out of the array when v is not needed any more in the function?
// No, because we cant move out a string from a vector,
// becuase that would leave the vector with an empty slot.  
// We cant copy strings out of vectors.
fn main() {
    let s1 = String::from("hello");
    let mut v = Vec::new();
    v.push(s1);
    let s2: String = v[0];
    println!("{}", s2);
}