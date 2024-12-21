import { Component, inject } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/User';

@Component({
  selector: 'app-senior',
  standalone: true,
  imports: [],
  templateUrl: './senior.component.html',
  styleUrl: './senior.component.css'
})
export class SeniorComponent {
  private router = inject(Router)
  private navig = this.router.getCurrentNavigation()
  user :User = new User()
  usrname = ""
  
  ngOnInit(): void {
      if(this.navig?.extras?.state){
        this.user = this.navig.extras.state['user']
        this.usrname = this.user.username
      }
  }

  logout(){
    this.router.navigate([''])
  }

  viewProfile(){
    this.router.navigate([`/viewProfile`], {
      state: {user: this.user}
    })
  }

  requestAService(){
    this.router.navigate([`/request`], {
      state: {user: this.user}
    })
  }

  settings(){
    this.router.navigate([`/settings`], {
      state: {user: this.user}
    })
  }

}
