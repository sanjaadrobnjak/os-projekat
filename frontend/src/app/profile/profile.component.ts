import { Component, inject, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/User';

@Component({
  selector: 'app-profile',
  standalone: true,
  imports: [],
  templateUrl: './profile.component.html',
  styleUrl: './profile.component.css'
})
export class ProfileComponent implements OnInit{
  private router = inject(Router)
  private navig = this.router.getCurrentNavigation()
  user :User = new User()
  firstname=""
  lastname=""
  username=""    
  password=""
  address=""
  mail=""
  type=""
  subscription=""

  
  
    
  ngOnInit(): void {
    if(this.navig?.extras?.state){
      this.user = this.navig.extras.state['user']
      this.firstname=this.user.firstname
      this.lastname= this.user.lastname
      this.username=this.user.username
      this.password=this.user.password
      this.address=this.user.address
      this.mail=this.user.mail
      this.type = this.user.type
      this.subscription=this.user.subscription
    }
  }

  back(){
    this.router.navigate([`/${this.user.type}`], {
      state: {user: this.user}
    })
  }

}
