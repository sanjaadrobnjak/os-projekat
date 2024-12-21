import { Component, inject, OnInit } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { User } from '../models/User';
import { UserService } from '../services/user.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-register',
  standalone: true,
  imports: [FormsModule],
  templateUrl: './register.component.html',
  styleUrl: './register.component.css'
})
export class RegisterComponent implements OnInit{
  ngOnInit(): void {
    this.type = "senior"
    this.subscription = "yearly"
  }

  private service = inject(UserService)
  private router = inject(Router)
  u: User = new User()
  firstname=""
  lastname=""
  username=""
  password=""
  address=""
  mail=""
  type=""
  subscription=""

  message=""

  register(){
    this.message = ""

    if (!this.firstname || this.firstname.trim() === '') {
      this.message = 'Firstname is required.\n';
      return;
    }
  
    if (!this.lastname || this.lastname.trim() === '') {
      this.message = 'Lastname is required.\n';
      return;
    }
  
    if (!this.username || this.username.trim() === '') {
      this.message = 'Username is required.\n';
      return;
    }
  
    if (!this.password || this.password.trim() === '') {
      this.message = 'Password is required.\n';
      return;
    }
  
    if (!this.address || this.address.trim() === '') {
      this.message = 'Address is required.\n';
      return;
    }
  
    if (!this.mail || this.mail.trim() === '') {
      this.message = 'Mail is required.\n';
      return;

    } else if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(this.mail)) {
      this.message += 'Mail must be a valid email address.\n';
      return;
    }
    
    this.u.firstname = this.firstname
    this.u.lastname = this.lastname
    this.u.username = this.username
    this.u.password = this.password
    this.u.address = this.address
    this.u.mail = this.mail
    this.u.type = this.type
    this.u.subscription = this.subscription
    
    this.service.register(this.u).subscribe(data=>{
      if(data==null){
        this.message="That username is already in use"
      } else{
        this.router.navigate([`/${this.type}`], {
          state: {user: this.u}
        });
      }
    })
  }
}
