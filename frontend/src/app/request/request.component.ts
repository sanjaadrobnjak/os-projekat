import { Component, inject, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/User';

@Component({
  selector: 'app-request',
  standalone: true,
  imports: [],
  templateUrl: './request.component.html',
  styleUrl: './request.component.css'
})
export class RequestComponent implements OnInit{
  private router = inject(Router)
  private navig = this.router.getCurrentNavigation()
  user :User = new User()

  ngOnInit(): void {
    if(this.navig?.extras?.state){
      this.user = this.navig.extras.state['user']
    }
  }

  back(){
    this.router.navigate([`/${this.user.type}`], {
      state: {user: this.user}
    })
  }

}
