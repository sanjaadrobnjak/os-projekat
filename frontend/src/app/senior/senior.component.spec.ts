import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SeniorComponent } from './senior.component';

describe('SeniorComponent', () => {
  let component: SeniorComponent;
  let fixture: ComponentFixture<SeniorComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SeniorComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(SeniorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
