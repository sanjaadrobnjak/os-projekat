import { Component, inject, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { HelperService } from '../services/helper.service';
import { User } from '../models/User';

@Component({
  selector: 'app-helper',
  standalone: true,
  imports: [],
  templateUrl: './helper.component.html',
  styleUrl: './helper.component.css'
})
export class HelperComponent implements OnInit{
  private router = inject(Router)
  private navig = this.router.getCurrentNavigation()
  private service = inject(HelperService)
  ngOnInit(): void {
    if(this.navig?.extras?.state){
      this.user = this.navig.extras.state['user']
    }
  }

  user: User = new User()

  logout(){
    this.router.navigate([''])
  }

  showServices(){
    this.router.navigate([`/provide`], {
      state: {user: this.user}
    });
  }

  viewProfile(){
    this.router.navigate([`/viewProfile`], {
      state: {user: this.user}
    })
  }

  settings(){
    this.router.navigate([`/settings`], {
      state: {user: this.user}
    })
  }

}
